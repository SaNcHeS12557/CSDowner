#include "mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QPushButton>

#include <QApplication>
#include <QDebug>
#include <Windows.h>

// bool isRunningAsAdmin() {
//     BOOL isAdmin = FALSE;
//     PSID administratorsGroup = NULL;
//     SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

//     if (AllocateAndInitializeSid(
//             &NtAuthority,
//             2,
//             SECURITY_BUILTIN_DOMAIN_RID,
//             DOMAIN_ALIAS_RID_ADMINS,
//             0, 0, 0, 0, 0, 0,
//             &administratorsGroup))
//     {
//         CheckTokenMembership(NULL, administratorsGroup, &isAdmin);
//         FreeSid(administratorsGroup);
//     }

//     return (isAdmin == TRUE);
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // if (isRunningAsAdmin()) {
    //     qDebug() << "App is running with admin rights.";
    // } else {
    //     qDebug() << "App is not running with admin rights.";
    // }

    return a.exec();
}
