//
// Generated file, do not edit! Created by nedtool 5.4 from veins/modules/application/F2MD/mdMessages/BasicSafetyMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <veins/modules/application/f2mdVeinsApp/mdMessages/BasicSafetyMessage_m.h>

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace Veins {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(BasicSafetyMessage)

BasicSafetyMessage::BasicSafetyMessage(const char *name, short kind) : ::Veins::BaseFrame1609_4(name,kind)
{
    this->senderWidth = 0;
    this->senderLength = 0;
    this->senderMbType = 0;
    this->senderAttackType = 0;
    this->senderPseudonym = 0;
}

BasicSafetyMessage::BasicSafetyMessage(const BasicSafetyMessage& other) : ::Veins::BaseFrame1609_4(other)
{
    copy(other);
}

BasicSafetyMessage::~BasicSafetyMessage()
{
}

BasicSafetyMessage& BasicSafetyMessage::operator=(const BasicSafetyMessage& other)
{
    if (this==&other) return *this;
    ::Veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void BasicSafetyMessage::copy(const BasicSafetyMessage& other)
{
    this->senderPos = other.senderPos;
    this->senderPosConfidence = other.senderPosConfidence;
    this->senderSpeed = other.senderSpeed;
    this->senderSpeedConfidence = other.senderSpeedConfidence;
    this->senderHeading = other.senderHeading;
    this->senderHeadingConfidence = other.senderHeadingConfidence;
    this->senderWidth = other.senderWidth;
    this->senderLength = other.senderLength;
    this->senderMbType = other.senderMbType;
    this->senderAttackType = other.senderAttackType;
    this->senderGpsCoordinates = other.senderGpsCoordinates;
    this->senderPseudonym = other.senderPseudonym;
}

void BasicSafetyMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::Veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->senderPos);
    doParsimPacking(b,this->senderPosConfidence);
    doParsimPacking(b,this->senderSpeed);
    doParsimPacking(b,this->senderSpeedConfidence);
    doParsimPacking(b,this->senderHeading);
    doParsimPacking(b,this->senderHeadingConfidence);
    doParsimPacking(b,this->senderWidth);
    doParsimPacking(b,this->senderLength);
    doParsimPacking(b,this->senderMbType);
    doParsimPacking(b,this->senderAttackType);
    doParsimPacking(b,this->senderGpsCoordinates);
    doParsimPacking(b,this->senderPseudonym);
}

void BasicSafetyMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::Veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->senderPos);
    doParsimUnpacking(b,this->senderPosConfidence);
    doParsimUnpacking(b,this->senderSpeed);
    doParsimUnpacking(b,this->senderSpeedConfidence);
    doParsimUnpacking(b,this->senderHeading);
    doParsimUnpacking(b,this->senderHeadingConfidence);
    doParsimUnpacking(b,this->senderWidth);
    doParsimUnpacking(b,this->senderLength);
    doParsimUnpacking(b,this->senderMbType);
    doParsimUnpacking(b,this->senderAttackType);
    doParsimUnpacking(b,this->senderGpsCoordinates);
    doParsimUnpacking(b,this->senderPseudonym);
}

Coord& BasicSafetyMessage::getSenderPos()
{
    return this->senderPos;
}

void BasicSafetyMessage::setSenderPos(const Coord& senderPos)
{
    this->senderPos = senderPos;
}

Coord& BasicSafetyMessage::getSenderPosConfidence()
{
    return this->senderPosConfidence;
}

void BasicSafetyMessage::setSenderPosConfidence(const Coord& senderPosConfidence)
{
    this->senderPosConfidence = senderPosConfidence;
}

Coord& BasicSafetyMessage::getSenderSpeed()
{
    return this->senderSpeed;
}

void BasicSafetyMessage::setSenderSpeed(const Coord& senderSpeed)
{
    this->senderSpeed = senderSpeed;
}

Coord& BasicSafetyMessage::getSenderSpeedConfidence()
{
    return this->senderSpeedConfidence;
}

void BasicSafetyMessage::setSenderSpeedConfidence(const Coord& senderSpeedConfidence)
{
    this->senderSpeedConfidence = senderSpeedConfidence;
}

Coord& BasicSafetyMessage::getSenderHeading()
{
    return this->senderHeading;
}

void BasicSafetyMessage::setSenderHeading(const Coord& senderHeading)
{
    this->senderHeading = senderHeading;
}

Coord& BasicSafetyMessage::getSenderHeadingConfidence()
{
    return this->senderHeadingConfidence;
}

void BasicSafetyMessage::setSenderHeadingConfidence(const Coord& senderHeadingConfidence)
{
    this->senderHeadingConfidence = senderHeadingConfidence;
}

double BasicSafetyMessage::getSenderWidth() const
{
    return this->senderWidth;
}

void BasicSafetyMessage::setSenderWidth(double senderWidth)
{
    this->senderWidth = senderWidth;
}

double BasicSafetyMessage::getSenderLength() const
{
    return this->senderLength;
}

void BasicSafetyMessage::setSenderLength(double senderLength)
{
    this->senderLength = senderLength;
}

int BasicSafetyMessage::getSenderMbType() const
{
    return this->senderMbType;
}

void BasicSafetyMessage::setSenderMbType(int senderMbType)
{
    this->senderMbType = senderMbType;
}

int BasicSafetyMessage::getSenderAttackType() const
{
    return this->senderAttackType;
}

void BasicSafetyMessage::setSenderAttackType(int senderAttackType)
{
    this->senderAttackType = senderAttackType;
}

Coord& BasicSafetyMessage::getSenderGpsCoordinates()
{
    return this->senderGpsCoordinates;
}

void BasicSafetyMessage::setSenderGpsCoordinates(const Coord& senderGpsCoordinates)
{
    this->senderGpsCoordinates = senderGpsCoordinates;
}

unsigned long BasicSafetyMessage::getSenderPseudonym() const
{
    return this->senderPseudonym;
}

void BasicSafetyMessage::setSenderPseudonym(unsigned long senderPseudonym)
{
    this->senderPseudonym = senderPseudonym;
}

class BasicSafetyMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BasicSafetyMessageDescriptor();
    virtual ~BasicSafetyMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(BasicSafetyMessageDescriptor)

BasicSafetyMessageDescriptor::BasicSafetyMessageDescriptor() : omnetpp::cClassDescriptor("Veins::BasicSafetyMessage", "Veins::BaseFrame1609_4")
{
    propertynames = nullptr;
}

BasicSafetyMessageDescriptor::~BasicSafetyMessageDescriptor()
{
    delete[] propertynames;
}

bool BasicSafetyMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BasicSafetyMessage *>(obj)!=nullptr;
}

const char **BasicSafetyMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BasicSafetyMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BasicSafetyMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount() : 12;
}

unsigned int BasicSafetyMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *BasicSafetyMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderPos",
        "senderPosConfidence",
        "senderSpeed",
        "senderSpeedConfidence",
        "senderHeading",
        "senderHeadingConfidence",
        "senderWidth",
        "senderLength",
        "senderMbType",
        "senderAttackType",
        "senderGpsCoordinates",
        "senderPseudonym",
    };
    return (field>=0 && field<12) ? fieldNames[field] : nullptr;
}

int BasicSafetyMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPos")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPosConfidence")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderSpeed")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderSpeedConfidence")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderHeading")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderHeadingConfidence")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderWidth")==0) return base+6;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderLength")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderMbType")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderAttackType")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderGpsCoordinates")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPseudonym")==0) return base+11;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BasicSafetyMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "Coord",
        "Coord",
        "Coord",
        "Coord",
        "Coord",
        "Coord",
        "double",
        "double",
        "int",
        "int",
        "Coord",
        "unsigned long",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : nullptr;
}

const char **BasicSafetyMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BasicSafetyMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BasicSafetyMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BasicSafetyMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BasicSafetyMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSenderPos(); return out.str();}
        case 1: {std::stringstream out; out << pp->getSenderPosConfidence(); return out.str();}
        case 2: {std::stringstream out; out << pp->getSenderSpeed(); return out.str();}
        case 3: {std::stringstream out; out << pp->getSenderSpeedConfidence(); return out.str();}
        case 4: {std::stringstream out; out << pp->getSenderHeading(); return out.str();}
        case 5: {std::stringstream out; out << pp->getSenderHeadingConfidence(); return out.str();}
        case 6: return double2string(pp->getSenderWidth());
        case 7: return double2string(pp->getSenderLength());
        case 8: return long2string(pp->getSenderMbType());
        case 9: return long2string(pp->getSenderAttackType());
        case 10: {std::stringstream out; out << pp->getSenderGpsCoordinates(); return out.str();}
        case 11: return ulong2string(pp->getSenderPseudonym());
        default: return "";
    }
}

bool BasicSafetyMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 6: pp->setSenderWidth(string2double(value)); return true;
        case 7: pp->setSenderLength(string2double(value)); return true;
        case 8: pp->setSenderMbType(string2long(value)); return true;
        case 9: pp->setSenderAttackType(string2long(value)); return true;
        case 11: pp->setSenderPseudonym(string2ulong(value)); return true;
        default: return false;
    }
}

const char *BasicSafetyMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(Coord));
        case 1: return omnetpp::opp_typename(typeid(Coord));
        case 2: return omnetpp::opp_typename(typeid(Coord));
        case 3: return omnetpp::opp_typename(typeid(Coord));
        case 4: return omnetpp::opp_typename(typeid(Coord));
        case 5: return omnetpp::opp_typename(typeid(Coord));
        case 10: return omnetpp::opp_typename(typeid(Coord));
        default: return nullptr;
    };
}

void *BasicSafetyMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSenderPos()); break;
        case 1: return (void *)(&pp->getSenderPosConfidence()); break;
        case 2: return (void *)(&pp->getSenderSpeed()); break;
        case 3: return (void *)(&pp->getSenderSpeedConfidence()); break;
        case 4: return (void *)(&pp->getSenderHeading()); break;
        case 5: return (void *)(&pp->getSenderHeadingConfidence()); break;
        case 10: return (void *)(&pp->getSenderGpsCoordinates()); break;
        default: return nullptr;
    }
}

} // namespace Veins

