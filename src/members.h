/**
 * @file members.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _MEMBERS_H_
#define _MEMBERS_H_

#include <QString>
#include <QList>

class Member {
public:
    enum Visibility {
        Private,
        Protected,
        Public
    };

    Visibility visibility() const { return _visibility; }
    void setVisibility(Visibility visibility) { _visibility = visibility; }

    bool isStatic() const { return _staticFlag; }
    void setStatic(bool flag) { _staticFlag = flag; }

    QString name() const { return _name; }
    void setName(const QString& name) { _name = name; }

    static QString visibilityText(Visibility visibility) { return visibility == Public ? "+" : (visibility == Protected ? "#" : "-"); }
    static Visibility visibilityFromText(QString str, bool* ok = NULL) {
        if (ok) *ok = true;
        return str == "+" ? Public : (str == "#" ? Protected : (str == "-" ? Private : (ok ? *ok = false, Public : Public)));
    }

    virtual QString getUmlText() const = 0;
protected:
    Member(const QString& name, Visibility visibility, bool isStatic = false)
        : _staticFlag(isStatic), _visibility(visibility), _name(name) { }

    QString visibilityText() const { return visibilityText(_visibility); }
private:
    bool _staticFlag;
    Visibility _visibility;
    QString _name;
};

class Attribute : public Member {
public:
    Attribute(const QString& name, Visibility visibility,
              const QString& type, const QString& defaultValue = QString(),
              bool isStatic = false)
        : Member(name, visibility, isStatic), _defaultValue(defaultValue), _type(type) { }

    QString type() const { return _type; }
    void setType(const QString& type) { _type = type; }

    QString defaultValue() const { return _defaultValue; }
    void setDefaultValue(const QString& defVal) { _defaultValue = defVal; }

    virtual QString getUmlText() const {
        QString str = QString("%1 %2 : %3").arg(visibilityText(), name(), type());
        if (!_defaultValue.isEmpty())
            str += QString(" = %1").arg(defaultValue());
        return str;
    }
private:
    QString _defaultValue;
    QString _type;
};

class Operation : public Member {
public:
    struct Parameter {
        Parameter() { }
        Parameter(const QString& name, const QString& type) : name(name), type(type) { }
        QString name;
        QString type;
    };

    Operation(const QString& name, Visibility visibility, const QString& returnType, bool isAbstract = false, bool isStatic = false)
        : Member(name, visibility, isStatic), _returnType(returnType), abstractFlag(isAbstract) { }

    QString returnType() const { return _returnType; }
    void setReturnType(const QString& retType) { _returnType = retType; }

    bool isAbstract() const { return abstractFlag; }
    void setAbstract(bool flag) { abstractFlag = flag; }

    void addParameter(const Parameter& param) { parameterList.append(param); }
    void addParameter(const QString& name, const QString& type) { addParameter(Parameter(name, type)); }
    void addParameters(const QList<Parameter>& parameters) { parameterList.append(parameters); }

    void removeAllParameters() { parameterList.clear(); }

    QList<Parameter> getParameters() const { return parameterList; }
    QList<Parameter>& getParameters() { return parameterList; }

    virtual QString getUmlText() const {
        QString str = QString("%1 %2(%3)").arg(visibilityText(), name(), parametersText());
        if (!_returnType.isEmpty() && _returnType != "void")
            str += QString(" : %1").arg(returnType());
        return str;
    }
private:
    QString parametersText() const {
        bool first = true;
        QString ret;
        foreach (const Parameter& p, parameterList) {
            if (first) {
                ret += p.name + " : " + p.type;
                first = false;
            } else
                ret += ", " + p.name + " : " + p.type;
        }
        return ret;
    }

    QString _returnType;
    QList<Parameter> parameterList;
    bool abstractFlag;
};

#endif // _MEMBERS_H_
