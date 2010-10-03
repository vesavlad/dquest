/**
 * @author Ben Lau
 */

#ifndef DQCLAUSE_H
#define DQCLAUSE_H

/** @file
  @brief Header file for DQClause

 */

#include <QList>
#include <QVariant>
#include <QVector>

/// The clause of a column definition

class DQClause
{
public:
    enum Type{
        UNIQUE = 0,
        DEFAULT,
        NOT_NULL,

        /* Extra */
        PRIMARY_KEY,
        FOREIGN_KEY,
        LAST
    };

    DQClause();
    DQClause(const DQClause& other);

    DQClause(DQClause::Type type);
    DQClause(Type type, QVariant value);


    /// TRUE if this type of clause flag is set
    bool testFlag(Type);

    /// Get the value of the clause type
    QVariant flag(Type);

    void setFlag(Type type,QVariant val = true);

    /// | operation overloading
    /**
      @remarks If both of the flag is set in two clause object. It will use the value specified in second clause
     */
    DQClause operator|(const DQClause& other);

private:
    void init();
    QMap<Type,QVariant> m_flags;
};

/// "Unique" clause
#define DQUnique DQClause(DQClause::UNIQUE)

/// "NotNull" clause
#define DQNotNull DQClause(DQClause::NOT_NULL)

/// The "Default" clause
#define DQDefault(value) DQClause(DQClause::DEFAULT,value)

/// Encode the string
QString dqEscape(QString val,bool trimStrings = false);

#endif // DQCLAUSE_H
