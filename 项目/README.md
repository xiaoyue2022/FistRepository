# TCP 文件传输项目(飞鸟系统)报告

## 0.Github链接以及项目架构

**GitHub 仓库地址**：https://github.com/xiaoyue2022/FistRepository.git  

项目架构：

<img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250605144047730.png" alt="image-20250605144047730" style="zoom:50%;" />

详细目录结构如下：

```
├── README.md                 # 项目报告 (即本文档)
├── client/                   # 客户端代码目录
│   ├── Makefile              # (通常由 qmake 生成)
│   ├── bin/                  # 存放编译后的客户端可执行文件
│   │   └── tcpclient         # 客户端可执行文件
│   ├── build/                # 编译输出目录 (例如 build-tcpclient-...)
│   │   ├── moc/              # moc 生成的文件
│   │   ├── obj/              # 编译生成的对象文件
│   │   └── ui/               # ui 文件生成的头文件 (ui_*.h)
│   ├── include/              # 客户端头文件目录
│   │   └── clientwidget.h    # 客户端主窗口头文件
│   ├── src/                  # 客户端源文件目录
│   │   ├── clientmain.cpp    # 客户端 main 函数
│   │   └── clientwidget.cpp  # 客户端主窗口实现
│   ├── tcpclient.pro         # Qt 项目文件 (qmake)
│   └── ui/                   # Qt Designer UI 文件目录
│       └── clientwidget.ui   # 客户端主窗口 UI 定义
└── server/                   # 服务器端代码目录
    ├── bin/                  # 存放编译后的服务器可执行文件
    ├── build/                # 编译输出目录 (例如 build-tcpserver-...)
    │   ├── moc/              # moc 生成的文件
    │   ├── obj/              # 编译生成的对象文件
    │   └── ui/               # ui 文件生成的头文件 (ui_*.h)
    ├── include/              # 服务器头文件目录
    │   ├── accesscontrol.h   # 访问控制模块头文件
    │   ├── auditlog.h        # 审计日志模块头文件
    │   ├── authmanager.h     # 认证管理模块头文件
    │   ├── loginwidget.h     # 登录窗口头文件
    │   ├── securefiletransfer.h # 安全文件传输模块头文件
    │   └── widget.h          # 服务器主窗口头文件
    ├── src/                  # 服务器源文件目录
    │   ├── accesscontrol.cpp # 访问控制模块实现
    │   ├── auditlog.cpp      # 审计日志模块实现
    │   ├── authmanager.cpp   # 认证管理模块实现
    │   ├── loginwidget.cpp   # 登录窗口实现
    │   ├── main.cpp          # 服务器 main 函数
    │   ├── securefiletransfer.cpp # 安全文件传输模块实现
    │   └── widget.cpp        # 服务器主窗口实现
    ├── tcpserver.pro         # Qt 项目文件 (qmake)
    └── ui/                   # Qt Designer UI 文件目录
        ├── loginwidget.ui    # 登录窗口 UI 定义
        └── widget.ui         # 服务器主窗口 UI 定义
```

**关键模块说明:**

*   **客户端 (`client/`)**: 负责用户界面、与服务器建立连接、发送消息、接收文件。
    *   `clientwidget`: 核心UI和逻辑类。
*   **服务器 (`server/`)**: 负责监听连接、用户认证、权限管理、文件传输、审计日志。
    *   `loginwidget`: 处理用户登录和注册的UI和逻辑。
    *   `widget`: 服务器主窗口，处理客户端连接、消息和文件操作的UI和主要逻辑。
    *   `authmanager`: 负责用户认证、密码管理、用户锁定机制，与SQLite数据库 (`auth.db`) 交互。
    *   `accesscontrol`: 实现基于角色的访问控制 (RBAC) 和权限管理，ACL数据存储在加密的 `acl.dat` 文件中。
    *   `auditlog`: 负责记录关键操作到加密的 `audit.log` 文件，并提供日志完整性保护。
    *   `securefiletransfer`: 实现使用国密算法 (SM2, SM3, SM4) 的安全文件传输，包括密钥协商（推测）、数据加密和完整性校验。

## 1. 功能列表和详细功能解释

### 1.1 客户端功能 (`clientwidget`)

- **连接服务器**:

  - 用户可以输入服务器的IP地址和端口号。

  - 点击“连接”按钮与服务器建立TCP连接。

  - 实时状态窗口会显示连接状态和服务器发送的消息。

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110628564.png" alt="image-20250606110628564" style="zoom:80%;" />

- **文件接收**:

  - 连接成功后，客户端等待服务器发送文件。

  - 接收到文件头部信息（文件名、文件大小）后，开始接收文件数据。

  - 进度条显示文件接收进度。

  - 接收完成后，在状态窗口提示，并将文件保存在客户端运行目录下。

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110734592.png" alt="image-20250606110734592" style="zoom:80%;" />

- **消息发送与接收**:

  - 客户端可以向服务器发送文本消息。

  - 客户端可以接收服务器发送的文本消息。

  - 聊天消息框用于输入和显示聊天内容。

    ![image-20250606110825754](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110825754.png)

- **断开连接**:

  - 用户可以点击“断开连接”按钮与服务器断开TCP连接。

### 1.2 服务器功能 (`widget`, `loginwidget`)

- **用户认证 (`loginwidget`, `authmanager`)**:

  - 用户需要通过用户名和密码登录才能访问服务器功能。

    ![image-20250606104216583](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606104216583.png)

  - 支持用户注册新账号，成功注册后提示成功。

    ![image-20250606104649240](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606104649240.png)

  - 登录失败次数过多会导致账户锁定一段时间。

    ![image-20250606104322928](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606104322928.png)

- **服务器管理 (`widget`)**:

  - 管理员可以启动和停止TCP服务器，并指定监听端口。

  - 实时状态窗口显示客户端连接状态、文件传输信息和收到的消息。

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606105919690.png" alt="image-20250606105919690" style="zoom:80%;" />

- **文件发送 (`widget`, `securefiletransfer`)**:

  - 服务器可以向已连接的客户端发送文件。

  - 管理员可以选择要发送的文件。

  - 文件传输使用安全的传输协议（详见报告安全功能部分）。

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110421252.png" alt="image-20250606110421252" style="zoom:80%;" />

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110509711.png" alt="image-20250606110509711" style="zoom:83%;" />

- **消息发送与接收 (`widget`)**:

  - 服务器可以向客户端发送文本消息。

  - 服务器可以接收客户端发送的文本消息。

    ![image-20250606110954438](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110954438.png)

- **权限管理 (`widget`, `accesscontrol`, `authmanager`)**:

  - 管理员可以管理用户角色（管理员、普通用户）。管理员需输入自己的用户名与密码，再输入待操作用户的用户名，选择角色类型，即可完成角色分配。

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606111121807.png" alt="image-20250606111121807" style="zoom:80%;" />

  - 普通用户只能访问其被授权的资源（`/home` 目录下的文件），访问其他目录文件则无法被访问，并弹出提示窗口。

    <img src="C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606110057998.png" alt="image-20250606110057998" style="zoom:80%;" />

- **审计日志 (`auditlog`)**:

  - 系统记录关键操作，如用户登录/登出、文件上传/下载、权限变更等。

  - 日志包含时间戳、用户ID、事件类型、资源路径、源IP和详细信息。

  - 日志具有完整性保护机制，终端打开日志如下。

    ![image-20250606131814279](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250606131814279.png)

------

## 2. 密码和安全功能

### 2.1 系统安全性设计

本系统采用了多层安全机制来保护数据和用户账户：

- **身份认证**: 强制用户登录，防止未授权访问。
- **密码安全**: 
  - 密码在存储和传输时不以明文形式出现。
  - 使用加盐哈希（SM3算法）存储用户密码，防止彩虹表攻击。
  - 登录尝试失败次数限制和账户锁定机制，防止暴力破解。
- **访问控制**: 
  - 基于角色的访问控制 (RBAC)，区分管理员和普通用户权限。
  - 基于属性的访问控制 (ABAC) 的雏形，例如普通用户只能访问特定目录。
  - 细粒度的权限管理，允许管理员控制用户对特定资源的读、写、删除等操作。
- **安全文件传输**: 
  - 文件传输过程使用国密算法（SM2、SM3、SM4）进行加密和完整性校验。
  - **数据加密**: 文件数据块使用SM4-CBC模式进行加密传输。
  - **完整性校验**: 使用SM3哈希算法校验文件在传输过程中的完整性。
- **审计日志**: 
  - 记录关键系统事件，用于事后追踪和分析。
  - 日志条目包含哈希链（理论上，用于保证日志不可篡改，但当前实现可能需要调整）。
  - 日志内容进行加密存储（SM4）。
- **防重放攻击**: `AuthManager::checkReplayAttack` 方法用于检查登录请求是否在允许的时间窗口内，以防止重放攻击 (尽管其实现细节未在提供的代码片段中完全展示)。
- **双因素认证 (2FA)**: `AccessControl` 和 `AuthManager` 包含请求和验证2FA的功能，主要用于管理员执行敏感操作（如权限修改）前的额外验证。

### 2.2 密码应用

- **用户登录 (`AuthManager::login`, `AuthManager::verifyPassword`)**: 
  - 用户输入的密码与存储在数据库中的盐值结合，计算SM3哈希值。
  - 将计算出的哈希值与数据库中存储的密码哈希进行比较。
- **用户注册 (`AuthManager::registerUser`)**: 
  - 生成随机盐值。
  - 用户密码与盐值结合，计算SM3哈希值。
  - 用户名、密码哈希和盐值存储到数据库。
- **管理员密码 (`widget.ui` 中的 `adminPasswordEdit`)**: 用于在权限管理界面验证管理员身份，执行分配角色等操作。其验证逻辑可能在 `Widget::on_assignRoleButton_clicked` 中，并最终调用 `AuthManager` 的相关方法，这些方法内部会进行密码校验或依赖于已登录的管理员会话。

### 2.3 安全功能实现

#### 2.3.1 身份认证 (`AuthManager`)

- **实现**: 
  1.  **数据库存储**: 用户名、加盐的SM3密码哈希、盐值存储在SQLite数据库 (`auth.db`) 的 `users` 表中。
  2.  **注册**: `registerUser` 函数负责生成盐值，计算密码哈希，并将用户信息存入数据库。同时，会通过 `AccessControl` 分配默认角色。
  3.  **登录**: `login` 函数首先检查用户是否被锁定。然后调用 `verifyPassword`，该函数从数据库获取用户的盐值和哈希，用输入密码和盐值重新计算哈希，并进行比较。
  4.  **失败锁定**: `recordFailedAttempt` 记录失败尝试。`isUserLocked` 检查失败次数是否超过 `MAX_FAILED_ATTEMPTS`，以及锁定时间（`LOCK_DURATION`）是否已过。
  5.  **防重放**: `checkReplayAttack` (具体实现未完全展示) 检查登录请求时间戳，防止旧请求被重用。
- **核心代码**: `AuthManager::registerUser`, `AuthManager::login`, `AuthManager::calculateHash`, `AuthManager::verifyPassword`, `AuthManager::recordFailedAttempt`, `AuthManager::isUserLocked`。

#### 2.3.2 访问控制 (`AccessControl`, `AuthManager`, `Widget`)

- **实现**: 
  1.  **角色与权限定义**: `AccessControl` 中定义了 `Role` (ADMIN, USER, GUEST) 和 `Permission` (READ, WRITE, DELETE, MODIFY_PERMISSIONS) 枚举。
  2.  **ACL存储**: `AccessControl::userAcl` (一个 `QMap<QString, ACLEntry>`) 存储用户的角色和资源权限。`ACLEntry` 包含用户的角色集合和每个资源的权限集合。ACL数据加密后保存在 `acl.dat` 文件中。
  3.  **角色分配**: `AccessControl::assignRole` 用于给用户分配角色。管理员通过 `Widget` UI调用 `AuthManager::assignRole`，后者再调用 `AccessControl` 的方法。
  4.  **权限授予/撤销**: `AccessControl::grantPermission` 和 `AccessControl::revokePermission` 用于管理用户对特定资源的权限。同样由管理员通过UI操作触发。
  5.  **权限检查**: `AccessControl::hasPermission` 检查用户是否对某资源有特定权限。管理员默认拥有所有权限。`Widget::checkUserRole` 和 `Widget::validateFileAccess` (部分逻辑) 在文件操作前调用此方法。
  6.  **上下文验证**: `AccessControl::validateAccess` 增加了基于时间 (`validateTimeBasedAccess`) 和IP (`validateIPBasedAccess`) 的访问控制，但具体策略实现未展示。
  7.  **双因素认证**: `AccessControl::requestTwoFactorAuth` 和 `verifyTwoFactorAuth` 用于敏感操作的额外验证。`AuthManager::verifyAdminAndTwoFactor` 在执行如授权等操作前调用这些方法。
- **核心代码**: `AccessControl::assignRole`, `AccessControl::grantPermission`, `AccessControl::hasPermission`, `AccessControl::saveACL`, `AccessControl::loadACL`, `AccessControl::requestTwoFactorAuth`, `AuthManager::checkPermission`, `Widget::on_assignRoleButton_clicked`。

#### 2.3.3 审计日志 (`AuditLog`)

- **实现**: 
  1.  **日志条目**: `AuditLogEntry` 结构体定义了日志的各个字段，包括时间戳、用户ID、事件类型 (`AuditEventType`)、资源路径、源IP、详情、前一个哈希和当前哈希。
  2.  **日志记录**: `AuditLog::logEvent` 是核心记录函数。它创建日志条目，计算哈希（理论上用于链接），加密条目，然后写入日志文件。
  3.  **哈希链**: `calculateSM3Hash` 用于计算日志条目的SM3哈希。理想情况下，`previousHash` 应该来自上一条日志的 `currentHash`，以形成链条，保证日志不可篡改。当前实现似乎将当前条目的数据也用于计算 `previousHash`，这需要修正以实现真正的哈希链。
  4.  **加密存储**: `encryptAndSaveEntry` 使用SM4算法加密序列化后的JSON格式日志条目，然后写入文件。`encryptWithSM4` 和 `generateSM4Key`/`generateIV` 负责加密逻辑。
  5.  **日志轮转与备份**: 当日志文件达到一定大小时 (`rotateLogFile`)，会进行轮转。`createBackup` 负责创建日志文件的备份。
  6.  **完整性验证**: `verifyIntegrity` (实现细节未完全展示) 用于检查日志链的完整性。
  7.  **异常检测**: `isAnomalous` (实现细节未完全展示) 用于检测可疑的日志行为并触发告警。
- **核心代码**: `AuditLog::logEvent`, `AuditLog::calculateSM3Hash`, `AuditLog::encryptAndSaveEntry`, `AuditLog::encryptWithSM4`.

## 3. 功能实现过程

### 3.1 客户端 (`clientwidget.cpp`)

- **连接服务器** (`on_pushButtonconnect_clicked`):

  ```cpp
  // d:\code\tcpfilev5\client\src\clientwidget.cpp
  void clientwidget::on_pushButtonconnect_clicked()
  {
      QString ipAdress = ui->lineEditip->text();
      quint16 port = ui->lineEditport->text().toInt();
      tcpSocket->connectToHost(QHostAddress(ipAdress),port);
      isStart = true; //连接成功
      file_mes = true; //默认连接成功可以发消息
      qDebug()<<"连接成功";
      //设置进度条
      ui->progress_01->setValue(0);
  }
  ```

- **处理接收到的数据** (connect `readyRead` signal):

  ```cpp
  // d:\code\tcpfilev5\client\src\clientwidget.cpp
  connect(tcpSocket, &QTcpSocket::readyRead,
      [=]()
      {
          qDebug()<<"isStart:"<<isStart;
          //取出接收的内容
          QByteArray buf = tcpSocket->readAll();
          //处理头部信息
          if(isStart == true){
              isStart = false;
              QString mark = QString(buf).section("##", 0, 0);
              qDebug()<<mark<<":"<<isStart;
              if(mark== "mesmark"){
                  file_mes = true;
              }else{
                  file_mes = false;
                  dealFileHead(&buf);
              }
          }else{
              qDebug()<<"执行处理函数";
              file_mes?dealMes(&buf):dealFile(&buf);
          }});
  ```

- **处理文件头部信息** (`dealFileHead`):

  ```cpp
  // d:\code\tcpfilev5\client\src\clientwidget.cpp
  void clientwidget::dealFileHead(QByteArray *buf)
  {
      qDebug()<<"处理文件头部信息";
      fileName = QString(*buf).section("##", 1, 1);
      fileSize = QString(*buf).section("##", 2, 2).toInt();
      recvSize = 0;   //已经接收文件大小
      file->setFileName(fileName);
      bool isOk = file->open(QIODevice::WriteOnly);
      // ... error handling and UI updates ...
  }
  ```

- **处理文件数据** (`dealFile`):

  ```cpp
  // d:\code\tcpfilev5\client\src\clientwidget.cpp
  void clientwidget::dealFile(QByteArray *buf)
  {
      qint64 len = file->write(*buf);
      if(len >0)
      {
          recvSize += len; //累计接收大小
      }
      ui->progress_01->setValue(recvSize/1024);
      if(recvSize == fileSize)
      {
          // ... UI updates, send "file done", close file ...
          isStart = true;
      }
  }
  ```

### 3.2 服务器 - 登录 (`loginwidget.cpp`, `authmanager.cpp`)

- **登录按钮点击** (`LoginWidget::on_loginButton_clicked`):

  ```cpp
  // d:\code\tcpfilev5\server\src\loginwidget.cpp
  void LoginWidget::on_loginButton_clicked()
  {
      QString username = ui->usernameEdit->text().trimmed();
      QString password = ui->passwordEdit->text();
      // ... input validation ...
      currentUsername = username;
      if (authManager->isUserLocked(username)) {
          // ... show locked message ...
          return;
      }
      if (authManager->login(username, password)) {
          // ... show success message, emit loginSuccessful ...
      } else {
          showMessage("Invalid username or password");
      }
  }
  ```

- **用户登录逻辑** (`AuthManager::login`):

  ```cpp
  // d:\code\tcpfilev5\server\src\authmanager.cpp
  bool AuthManager::login(const QString& username, const QString& password)
  {
      QMutexLocker locker(&mutex);
      if (isUserLocked(username)) {
          return false;
      }
      if (verifyPassword(username, password)) {
          resetFailedAttempts(username);
          // ... update last_login ...
          return true;
      } else {
          recordFailedAttempt(username);
          return false;
      }
  }
  ```

### 3.3 服务器 - 主逻辑 (`widget.cpp`)

- **新连接处理** (`Widget::newConnection`):

  ```cpp
  // d:\code\tcpfilev5\server\src\widget.cpp
  void Widget::newConnection() {
      tcpSocket = tcpServer->nextPendingConnection();
      // ... get IP and port, update UI ...
      connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::socketReadyRead);
      connect(tcpSocket, &QTcpSocket::stateChanged, this, &Widget::socketStateChanged);
  }
  ```

- **文件选择** (`Widget::on_pushButtonselect_clicked`):

  ```cpp
  // d:\code\tcpfilev5\server\src\widget.cpp
  void Widget::on_pushButtonselect_clicked()
  {
      // ... check login, determine startDir based on role ...
      QString filePath = QFileDialog::getOpenFileName(this, "open", startDir);
      if(filePath.isEmpty() == false){
          if (!checkUserRole(currentUser, filePath)) { /* ... handle permission error ... */ }
          // ... get file info, open file ...
          file_mes = false;
      } else { /* ... handle error ... */ }
  }
  ```

- **发送文件/消息** (`Widget::on_pushButtonsend_clicked`, `Widget::sendData`, `Widget::sendMes`):

  ```cpp
  // d:\code\tcpfilev5\server\src\widget.cpp
  void Widget::on_pushButtonsend_clicked()
  {
      if(file_mes == false){ //发文件
          // ... send file header (filename##filesize) ...
          timer.start(20); // Start timer to send data chunks
      } else { //发消息
          // ... send message header (mesmark##) ...
          sendMes();
      }
  }
  ```

### 3.4 服务器 - 访问控制 (`accesscontrol.cpp`)

- **检查权限** (`AccessControl::hasPermission`):

  ```cpp
  // d:\code\tcpfilev5\server\src\accesscontrol.cpp
  bool AccessControl::hasPermission(const QString& userId, const QString& resource, Permission perm) const {
      if (!userAcl.contains(userId)) {
          return false;
      }
      const auto& entry = userAcl[userId];
      if (entry.roles.contains(Role::ADMIN)) {
          return true; // Admins have all permissions
      }
      if (!entry.resourcePermissions.contains(resource)) {
          return false;
      }
      return entry.resourcePermissions[resource].contains(perm);
  }
  ```

- **分配角色** (`AccessControl::assignRole`):

  ```cpp
  // d:\code\tcpfilev5\server\src\accesscontrol.cpp
  bool AccessControl::assignRole(const QString& userId, Role role) {
      if (userAcl.contains(userId)) {
          userAcl[userId].roles.insert(role);
      } else {
          ACLEntry entry;
          entry.roles.insert(role);
          userAcl[userId] = entry;
      }
      return true;
  }
  ```

### 3.5 服务器 - 审计日志 (`auditlog.cpp`)

- **记录事件** (`AuditLog::logEvent`):

  ```cpp
  // d:\code\tcpfilev5\server\src\auditlog.cpp
  void AuditLog::logEvent(const QString& userId,
                         AuditEventType eventType,
                         const QString& resourcePath,
                         const QHostAddress& sourceIP,
                         const QString& details) {
      QMutexLocker locker(&mutex);
      AuditLogEntry entry;
      // ... populate entry fields ...
      entry.previousHash = calculateSM3Hash(entry); // Incorrect, should be hash of *previous* entry
      // ... rotate log file if needed ...
      if (!encryptAndSaveEntry(entry)) { /* ... handle error ... */ }
      if (isAnomalous(entry)) { /* ... emit security alert ... */ }
      createBackup();
  }
  ```

- **计算SM3哈希** (`AuditLog::calculateSM3Hash` - *Note: This seems to hash the current entry, not link to previous for a chain*):

  ```cpp
  // d:\code\tcpfilev5\server\src\auditlog.cpp
  QByteArray AuditLog::calculateSM3Hash(const AuditLogEntry& entry) {
      SM3_CTX ctx;
      unsigned char hash[32];
      sm3_init(&ctx);
      // ... update with previousHash (if available) and current entry data ...
      sm3_finish(&ctx, hash);
      return QByteArray((char*)hash, 32);
  }
  ```

### 3.6 服务器 - 安全文件传输 (`securefiletransfer.cpp`)

- **初始化会话** (`SecureFileTransfer::initializeSession`):

  ```cpp
  // d:\code\tcpfilev5\server\src\securefiletransfer.cpp
  QString SecureFileTransfer::initializeSession(const QString& filePath,
                                             const QByteArray& clientPublicKey) {
      QMutexLocker locker(&mutex);
      // ... check file existence ...
      TransferSession session;
      session.sessionId = generateSessionId();
      session.sm2PublicKey = clientPublicKey; // Store client's SM2 public key
      session.filePath = filePath;
      session.fileSize = file.size();
      session.sm4Key = generateSessionKey(); // Generate SM4 session key
      session.sm4IV = generateIV();          // Generate SM4 IV
      session.sm3Hash = calculateSM3Hash(filePath); // Calculate SM3 hash of the file
      activeSessions[session.sessionId] = session;
      return session.sessionId;
      // Note: The SM4 key should be encrypted with client's SM2 public key and sent to client.
  }
  ```

- **加密文件块** (`SecureFileTransfer::encryptFileBlock`):

  ```cpp
  // d:\code\tcpfilev5\server\src\securefiletransfer.cpp
  QByteArray SecureFileTransfer::encryptFileBlock(const QString& sessionId,
                                                const QByteArray& data) {
      QMutexLocker locker(&mutex);
      if (!activeSessions.contains(sessionId)) { return QByteArray(); }
      TransferSession& session = activeSessions[sessionId];
      // Encrypt with SM4-CBC
      QByteArray encrypted = encryptWithSM4CBC(data, session.sm4Key, session.sm4IV);
      // ... update progress, emit signals ...
      return encrypted;
  }
  ```

------

## 4. 数据库设计

项目主要使用SQLite数据库存储用户信息。

**数据库文件名**: `auth.db`

**表名**: `users`

| 列名            | 数据类型   | 约束                        | 描述                    |
| --------------- | ---------- | --------------------------- | ----------------------- |
| `username`      | `TEXT`     | `PRIMARY KEY`               | 用户名，唯一标识用户    |
| `password_hash` | `BLOB`     | `NOT NULL`                  | 加盐后使用SM3哈希的密码 |
| `salt`          | `BLOB`     | `NOT NULL`                  | 用于密码哈希的盐值      |
| `last_login`    | `DATETIME` |                             | 用户最后登录时间        |
| `created_at`    | `DATETIME` | `DEFAULT CURRENT_TIMESTAMP` | 用户账户创建时间        |

![image-20250530093951518](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20250530093951518.png)





