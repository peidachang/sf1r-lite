#ifndef SF1R_CONFIGURATION_MANAGER_COLLECTION_META_H
#define SF1R_CONFIGURATION_MANAGER_COLLECTION_META_H
/**
 * @file CollectionMeta.h
 * @date Created <2009-10-22 16:55:37>
 * @date Updated <2011-01-25 14:54:01>
 */
#include <common/type_defs.h>

#include <configuration-manager/PropertyConfig.h>
#include <configuration-manager/Acl.h>

#include <bundles/index/IndexBundleConfiguration.h>
#include <bundles/mining/MiningBundleConfiguration.h>

#include <util/ustring/UString.h>

#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <set>

namespace sf1r
{

class CollectionMeta
{
public:
    CollectionMeta();

    ~CollectionMeta() {}

    void setColId(collectionid_t id)
    {
        colId_ = id;
    }

    collectionid_t getColId() const
    {
        return colId_;
    }

    void setName(std::string name)
    {
        name_ = name;
    }

    const std::string& getName() const
    {
        return name_;
    }

    void setDateFormat(const std::string& format)
    {
        dateFormat_ = format;
    }

    const std::string& getDateFormat() const
    {
        return dateFormat_;
    }

    void setCollectionPath(const CollectionPath& collPath)
    {
        collPath_ = collPath;
    }

    const CollectionPath& getCollectionPath() const
    {
        return collPath_;
    }

    /**
     * @brief adds one property config to schema
     * @return \c false if the name already existed, \c true if really inserted.
     */
    bool insertProperty(const PropertyConfigBase& property)
    {
        return documentSchema_.insert(property).second;
    }

    /**
     * @brief erases by name
     * @return \c true if the property exists and has been erased.
     */
    bool eraseProperty(const std::string& name)
    {
        PropertyConfigBase config;
        config.propertyName_ = name;
        return documentSchema_.erase(config);
    }

    bool getProperty(PropertyConfigBase& config) const;

    bool getProperty(const std::string& name, PropertyConfigBase& config) const;

    bool getPropertyType(const std::string& name, PropertyDataType& type) const;

    const DocumentSchema& getDocumentSchema() const
    {
        return documentSchema_;
    }

    std::string toString() const;

    /**
     * @brief number property in schema.
     *
     * Property id are numbered from 1 and by the name lexical order.
     */
    void numberProperty();

    void aclAllow(const string& tokens)
    {
        acl_.allow(tokens);
    }

    void aclDeny(const string& tokens)
    {
        acl_.deny(tokens);
    }

    void deleteTokenFromAclAllow(const string& token)
    {
        acl_.deleteTokenFromAllow(token);
    }

    void deleteTokenFromAclDeny(const string& token)
    {
        acl_.deleteTokenFromDeny(token);
    }

    const Acl& getAcl() const
    {
        return acl_;
    }

public:
    /// @brief Collection ID
    collectionid_t colId_;

    /// @brief  The Collection's name given by the user
    std::string name_;

    std::string dateFormat_;

    CollectionPath collPath_;

    /// @brief The DocumentSchema of this Collection
    DocumentSchema documentSchema_;

    boost::shared_ptr<IndexBundleConfiguration> indexBundleConfig_;

    boost::shared_ptr<MiningBundleConfiguration> miningBundleConfig_;

    /// @brief Collection level acl_
    Acl acl_;
};

} // namespace sf1r

#endif // SF1R_CONFIGURATION_MANAGER_COLLECTION_META_H
