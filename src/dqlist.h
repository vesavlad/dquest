#ifndef DQLIST_H
#define DQLIST_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqabstractmodel.h>
#include <dqmodelmetainfo.h>
#include <dqsharedlist.h>

/// Storage of a list of model item instance
/**

  @remarks It is an explicity shared class
 */

template <class T>
class DQList : public DQSharedList
{
public:
    DQList() : DQSharedList() {
    }

    DQList(const DQList &rhs) : DQSharedList(rhs) {
    }

    DQList(const DQSharedList& rhs ) : DQSharedList(rhs) {

    }

    DQList &operator=(const DQList &rhs){
        DQSharedList::operator=( rhs);
        return *this;
    }

    ~DQList(){
    }

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).
    T* at(int i) const {
        DQAbstractModel* m = DQSharedList::at(i);
        if (m->metaInfo() != dqMetaInfo<T>() ) {
            qWarning() << QString("DQList::at() - Can not convert %1 to %2")
                          .arg(m->metaInfo()->className()).arg(dqMetaInfo<T>()->className());
            m = 0;
        }
        return (T*) m;
    }

    DQList& operator<<(const T& model){
        append(model);
        return *this;
    }


    /// Append a model to the list.
    /**
      @param model The input model. A copy of instance of the model will be stored to the list.
     */
    void append(const T& model) {
        T* t = new T(model);
        DQSharedList::append(t);
    }

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */

    void append(T* model) {
        DQSharedList::append(model);
    }

    operator DQSharedList() {
        DQSharedList res (*this);
        return res;
    }

};

template <class T>
inline QDebug operator<< (QDebug d, const DQList<T>& rhs ){
    QStringList record;
    int n = rhs.size();
    DQModelMetaInfo *metaInfo;
    for (int i = 0 ; i < n;i++) {
        DQAbstractModel *model = rhs.at(i);
        metaInfo = model->metaInfo();
        QStringList fields = metaInfo->fieldNameList();
        QStringList col;
        foreach (QString field,fields){
            QVariant value = metaInfo->value(model,field);
            if (value.isNull())
                continue;
            col << QString("%1=%2").arg(field).arg(value.toString());
        }

        QString res = QString("(%2)")
                      .arg(col.join(","));
        record << res;
    }

    metaInfo = dqMetaInfo<T>();

    d.nospace() << QString("%1[%2]")
                    .arg(metaInfo->className())
                    .arg(record.join(","));

    return d.space();
}


#endif // DQLIST_H