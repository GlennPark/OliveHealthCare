#include "chattingclient.h"
#include "chattingserver.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDataStream>
#include <QTcpSocket>
#include <QApplication>
#include <QThread>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>
#include <QButtonGroup>

#define BLOCK_SIZE      1024

ChattingClient::ChattingClient(QWidget *parent) : QWidget(parent), isSent(false)
{
    // 서버 주소를 LineEdit(serverAddress)을 통해 입력, 기본 주소 설정
    name = new QLineEdit(this);
    serverAddress = new QLineEdit(this);
    serverAddress->setText("127.0.0.1");
    //serverAddress->setInputMask("999.999.999.999;_");

    // 서버 주소의 정규 표현식
    QRegularExpression re("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionValidator validator(re);

    serverAddress->setPlaceholderText("Server IP Address");
    serverAddress->setValidator(&validator);

    // 서버 포트를 serverPort(LineEdit)을 통해 입력, 기본 포트 설정
    // 값이 변경될 때 까지 메시지 출력
    serverPort = new QLineEdit(this);
    serverPort->setText(QString::number(PORT_NUMBER));
    serverPort->setInputMask("00000;_");
    serverPort->setPlaceholderText("Server Port No");

    // connectButton(PushButton)을 통해 로그인, 채팅 서버로 연결
    connectButton = new QPushButton(tr("Log In"), this);

    // 이름, 서버주소, 서버포트,로그인 버튼을 레이아웃에 통합
    QHBoxLayout *serverLayout = new QHBoxLayout;
    serverLayout->addWidget(name);
    serverLayout->addStretch(1);
    serverLayout->addWidget(serverAddress);
    serverLayout->addWidget(serverPort);
    serverLayout->addWidget(connectButton);

    // 서버에서 오는 메시지를 읽기 전용으로 표시
    message = new QTextEdit(this);
    message->setReadOnly(true);

    // 서버로 보낼 메시지를 위한 위젯들
    inputLine = new QLineEdit(this);

    // inputLine(LineEdit)에서 키보드에서 엔터 키가 눌릴때 sendData 슬롯함수를 실행해서 데이터를 전송한다
    connect(inputLine, SIGNAL(returnPressed( )), this, SLOT(sendData( )));

    // inputLine(LineEdit)에서 키보드에서 엔터 키가 눌릴때 입력단을 비워준다
    connect(inputLine, SIGNAL(returnPressed( )), inputLine, SLOT(clear( )));

    sentButton = new QPushButton("Send", this);
    // sentButton(PushButton)클릭 시 sendData 슬롯함수를 실행해서 데이터를 전송한다
    connect(sentButton, SIGNAL(clicked( )), this, SLOT(sendData( )));

    // sentButton(PushButton)클릭 시 sendData 슬롯함수를 실행해서 입력단을 비워준다
    connect(sentButton, SIGNAL(clicked( )), inputLine, SLOT(clear( )));
    inputLine->setEnabled(false);
    sentButton->setEnabled(false);

    // inputLine과 sentButton을 통합하는 레이아웃
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputLine);
    inputLayout->addWidget(sentButton);

    // fileButton 클릭시 sendFile 슬롯함수를 실행해서 파일을 전송한다
    fileButton = new QPushButton("File Transfer", this);
    connect(fileButton, SIGNAL(clicked( )), this, SLOT(sendFile( )));
    fileButton->setDisabled(true);

    // quitButton(PushButton)클릭 시 qApp에서 quit()슬롯함수를 실행하여 로그아웃
    QPushButton* quitButton = new QPushButton("Log Out", this);
    connect(quitButton, SIGNAL(clicked( )), qApp, SLOT(quit( )));

    // fileButton과 quitButton을 통합하는 레이아웃
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(fileButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);

    // server, input, button 레이아웃과 message위젯을 통합하는 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(serverLayout);
    mainLayout->addWidget(message);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // 채팅 사용자를 연결하기 위한 소켓
    clientSocket = new QTcpSocket(this);

    // 소켓에 에러 발생시 디버깅 메시지 출력
    connect(clientSocket, &QAbstractSocket::errorOccurred,
            [=]{ qDebug( ) << clientSocket->errorString( ); });

    // 소켓이 읽을 준비가 되었다는 신호를 보내면 데이터를 전송받는다
    connect(clientSocket, SIGNAL(readyRead( )), this, SLOT(receiveData( )));
    // 소켓 연결 해제
    connect(clientSocket, SIGNAL(disconnected( )), this, SLOT(disconnect( )));

    // ChatClient 의 ID 설정 저장
    QSettings settings("ChatClient", "Chat Client");
    name->setText(settings.value("ChatClient/ID").toString());

    // 파일 소켓 생성
    fileClient = new QTcpSocket(this);
    connect(fileClient, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));

    // progressDialog : 파일 전송 시 진행 상황 표시후 자동 종료, 리셋
    progressDialog = new QProgressDialog(0);
    progressDialog->setAutoClose(true);
    progressDialog->reset();

    // connectButton(PushButton) 채팅 접속 버튼
    connect(connectButton, &QPushButton::clicked,
            [=]{

        // 입력한 서버 주소와 포트 서버 포트를 통해 Host에 연결
        if(connectButton->text() == tr("Log In"))
        {
            clientSocket->connectToHost(serverAddress->text( ),
                                        serverPort->text( ).toInt( ));
        // 접속 될 때 까지 대기
            clientSocket->waitForConnected();
        // 로그인을 위한 프로토콜 전송, name 을 읽기 전용으로 설정
            sendProtocol(Chat_Login, name->text().toStdString().data());
            connectButton->setText(tr("Chat in"));
            name->setReadOnly(true);
        }
        else if(connectButton->text() == tr("Chat in"))
        {
        // 채팅방 입장을 위한 프로토콜 전송, 채팅 입력창과 보내기, 파일 전송 활성화
            sendProtocol(Chat_In, name->text().toStdString().data());
            connectButton->setText(tr("Chat Out"));
            inputLine->setEnabled(true);
            sentButton->setEnabled(true);
            fileButton->setEnabled(true);
        }
        else if(connectButton->text() == tr("Chat Out"))
        {
        // 채팅방을 나가기 위한 프로토콜 전송, 채팅 입력창과 보내기, 파일 전송 비활성화
            sendProtocol(Chat_Out, name->text().toStdString().data());
            connectButton->setText(tr("Chat in"));
            inputLine->setDisabled(true);
            sentButton->setDisabled(true);
            fileButton->setDisabled(true);
        }
    } );

    setWindowTitle(tr("Chatting Client"));
}

// 채팅 로그아웃
void ChattingClient::closeEvent(QCloseEvent*)
{

    // 채팅 로그아웃을 위한 프로토콜 전송, 호스트와의 연결 종료
    sendProtocol(Chat_LogOut, name->text().toStdString().data());
    clientSocket->disconnectFromHost();
    // 소켓 Unconnected State 가 아니면 연결 종료까지 대기
    if(clientSocket->state() != QAbstractSocket::UnconnectedState)
        clientSocket->waitForDisconnected();
}

// 채팅 정보 입력받기
void ChattingClient::receiveData( )
{
    QTcpSocket *clientSocket = dynamic_cast<QTcpSocket *>(sender( ));
    if (clientSocket->bytesAvailable( ) > BLOCK_SIZE) return;
    QByteArray bytearray = clientSocket->read(BLOCK_SIZE);

    Chat_Status type;       // 채팅의 목적
    char data[1020];        // 전송되는 메시지/데이터
    memset(data, 0, 1020);

    QDataStream in(&bytearray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> type;
    in.readRawData(data, 1020);

    switch(type) {
    case Chat_Talk:
        message->append(QString(data));
        inputLine->setEnabled(true);
        sentButton->setEnabled(true);
        fileButton->setEnabled(true);
        break;
    case Chat_KickOut:
        QMessageBox::critical(this, tr("Chatting Client"), \
                              tr("Kick out from Server"));
        inputLine->setDisabled(true);
        sentButton->setDisabled(true);
        fileButton->setDisabled(true);
        name->setReadOnly(false);
        break;
    case Chat_Invite:
        QMessageBox::critical(this, tr("Chatting Client"), \
                              tr("Invited from Server"));
        inputLine->setEnabled(true);
        sentButton->setEnabled(true);
        fileButton->setEnabled(true);
        name->setReadOnly(true);
        break;
    };
}

void ChattingClient::disconnect( )
{
    QMessageBox::critical(this, tr("Chatting Client"), \
                          tr("Disconnect from Server"));
    inputLine->setEnabled(false);
    name->setReadOnly(false);
    sentButton->setEnabled(false);
    connectButton->setText(tr("Log in"));
}

void ChattingClient::sendProtocol(Chat_Status type, char* data, int size)
{
    QByteArray dataArray;           // 소켓으로 보낼 데이터를 채우고
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << type;
    out.writeRawData(data, size);
    clientSocket->write(dataArray);     // 서버로 전송
    clientSocket->flush();
    while(clientSocket->waitForBytesWritten());
}

void ChattingClient::sendData(  )
{
    QString str = inputLine->text( );
    if(str.length( )) {
        QByteArray bytearray;
        bytearray = str.toUtf8( );
        message->append("<font color=red>나</font> : " + str);
        sendProtocol(Chat_Talk, bytearray.data());
    }
}

void ChattingClient::goOnSend(qint64 numBytes) // Start sending file content
{
    byteToWrite -= numBytes; // Remaining data size
    outBlock = file->read(qMin(byteToWrite, numBytes));
    fileClient->write(outBlock);

    progressDialog->setMaximum(totalSize);
    progressDialog->setValue(totalSize-byteToWrite);

    if (byteToWrite == 0) { // Send completed
        qDebug("File sending completed!");
        progressDialog->reset();
    }
}

void ChattingClient::sendFile() // Open the file and get the file name (including path)
{
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QString filename = QFileDialog::getOpenFileName(this);
    file = new QFile(filename);
    file->open(QFile::ReadOnly);

    qDebug() << QString("file %1 is opened").arg(filename);
    progressDialog->setValue(0); // Not sent for the first time

    if (!isSent) { // Only the first time it is sent, it happens when the connection generates the signal connect
        fileClient->connectToHost(serverAddress->text( ),
                                  serverPort->text( ).toInt( ) + 1);
        isSent = true;
    }

    // When sending for the first time, connectToHost initiates the connect signal to call send, and you need to call send after the second time

    byteToWrite = totalSize = file->size(); // The size of the remaining data
    loadSize = 1024; // The size of data sent each time

    QDataStream out(&outBlock, QIODevice::WriteOnly);
    out << qint64(0) << qint64(0) << filename;

    totalSize += outBlock.size(); // The total size is the file size plus the size of the file name and other information
    byteToWrite += outBlock.size();

    out.device()->seek(0); // Go back to the beginning of the byte stream to write a qint64 in front, which is the total size and file name and other information size
    out << totalSize << qint64(outBlock.size());

    fileClient->write(outBlock); // Send the read file to the socket

    progressDialog->setMaximum(totalSize);
    progressDialog->setValue(totalSize-byteToWrite);
    progressDialog->show();

    qDebug() << QString("Sending file %1").arg(filename);
}


ChattingClient::~ChattingClient()
{

}
