/*
 * Copyright (c) 2008 - 2009  Dario Freddi <drf@chakra-project.org>
 *               2010 - 2011  Drake Justice <djustice@chakra-project.com>
 *                      2013  Manuel tortosa <manutortosa@chakra-project.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef USERWIDGET_H
#define USERWIDGET_H

#include "avatardialog.h"
#include "ui_userwidget.h"

#include <QLineEdit>
#include <QValidator>
#include <QRegExp>
#include <kmessagewidget.h>



class UsernameValidator: public QRegExpValidator
{
    Q_OBJECT
  
public:
    UsernameValidator(QRegExp exp);
    void fixup ( QString & input ) const ;
    QValidator::State validate ( QString & input, int & pos ) const;
    
signals:
    void invalidSymbolEntered(const QString &) const; // const is needed, because the signal is emitted in a const method
    void textIsValidAgain() const;

private:
    QStringList m_badNames;
};

class UserWidget : public QWidget
{
    Q_OBJECT

public:
    UserWidget(int, QWidget *parent = 0);
    virtual ~UserWidget();

    QLineEdit *loginLine;
    QString login;
    QString password;
    QString rootPassword;
    QString avatar;
    QString name;

    bool autoLogin;
    bool useRootPw;
    bool useUserPw;
    bool passwordsMatch;
    bool passwordsEmpty;
    bool rootPasswordsMatch;
    bool rootPasswordsEmpty;

    int number;
    void setNumber(int i) { number = i; }
    /** can be used to check if the username is valid */
    bool isUserNameValid();

public slots:
    void setLogin(const QString&);
    void setPassword(const QString&);
    void setRootPassword(const QString&);
    void setAvatar(const QString&);
    void setName(const QString&);
    void setAutoLogin(const QString&);
    void setUseRootPassword(const QString&);
    void showUsernameWarning(const QString&);
    void hideUsernameWarning();

signals:
    void addUserClicked();
    void removeUserClicked(int);
    void autoLoginToggled(int);

private slots:
    void showDetails();
    void showRootPw(bool);

    void emitRemove();

    void avatarClicked();
    void autoLoginToggled();
    void useUserPwToggled(bool);

    void testFields();
    void updatePasswordStrengthBar(const QString&);

private:
    Ui::UserWidget ui;
    AvatarDialog *m_avatarDialog;
    KMessageWidget *m_messageWidget;
    UsernameValidator *m_validator;
};

#endif /* USERWIDGET_H */
