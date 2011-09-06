#include "GroupCounterLabelBuilder.h"
#include "group_manager.h"
#include "StringGroupCounter.h"
#include "StringGroupLabel.h"
#include "NumericGroupCounter.h"
#include "NumericGroupLabel.h"

#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

NS_FACETED_BEGIN

GroupCounterLabelBuilder::GroupCounterLabelBuilder(
    const schema_type& schema,
    const GroupManager* groupManager
)
    : schema_(schema)
    , groupManager_(groupManager)
{
}

PropertyDataType GroupCounterLabelBuilder::getPropertyType_(const std::string& prop) const
{
    PropertyConfigBase config;
    config.propertyName_ = prop;

    schema_type::const_iterator it = schema_.find(config);
    if (it != schema_.end())
        return it->propertyType_;

    return UNKNOWN_DATA_PROPERTY_TYPE;
}

GroupCounter* GroupCounterLabelBuilder::createGroupCounter(const std::string& prop) const
{
    GroupCounter* counter = NULL;

    switch(getPropertyType_(prop))
    {
    case STRING_PROPERTY_TYPE:
        counter = createStringCounter(prop);
        break;

    case INT_PROPERTY_TYPE:
        counter = new NumericGroupCounter<int64_t>(groupManager_->getPropertyTable(prop));
        break;

    case UNSIGNED_INT_PROPERTY_TYPE:
        counter = new NumericGroupCounter<uint64_t>(groupManager_->getPropertyTable(prop));
        break;

    case FLOAT_PROPERTY_TYPE:
        counter = new NumericGroupCounter<float>(groupManager_->getPropertyTable(prop));
        break;

    case DOUBLE_PROPERTY_TYPE:
        counter = new NumericGroupCounter<double>(groupManager_->getPropertyTable(prop));
        break;

    default:
        LOG(ERROR) << "unknown property type " << prop;
        break;
    }

    return counter;
}

GroupCounter* GroupCounterLabelBuilder::createStringCounter(const std::string& prop) const
{
    GroupCounter* counter = NULL;

    const PropValueTable* pvTable = groupManager_->getPropValueTable(prop);
    if (pvTable)
    {
        counter = new StringGroupCounter(*pvTable);
    }
    else
    {
        LOG(ERROR) << "group index file is not loaded for group property " << prop;
    }

    return counter;
}

GroupLabel* GroupCounterLabelBuilder::createGroupLabel(const GroupParam::GroupLabel& labelParam) const
{
    GroupLabel* label = NULL;

    const std::string& prop = labelParam.first;
    switch(getPropertyType_(prop))
    {
    case STRING_PROPERTY_TYPE:
        label = createStringLabel(labelParam);
        break;

    case INT_PROPERTY_TYPE:
        label = new NumericGroupLabel<int64_t>(groupManager_->getPropertyTable(prop), boost::lexical_cast<int64_t>(labelParam.second[0]));
        break;

    case UNSIGNED_INT_PROPERTY_TYPE:
        label = new NumericGroupLabel<uint64_t>(groupManager_->getPropertyTable(prop), boost::lexical_cast<uint64_t>(labelParam.second[0]));
        break;

    case FLOAT_PROPERTY_TYPE:
        label = new NumericGroupLabel<float>(groupManager_->getPropertyTable(prop), boost::lexical_cast<float>(labelParam.second[0]));
        break;

    case DOUBLE_PROPERTY_TYPE:
        label = new NumericGroupLabel<double>(groupManager_->getPropertyTable(prop), boost::lexical_cast<double>(labelParam.second[0]));
        break;

    default:
        LOG(ERROR) << "unknown property type " << prop;
        break;
    }

    return label;
}

GroupLabel* GroupCounterLabelBuilder::createStringLabel(const GroupParam::GroupLabel& labelParam) const
{
    GroupLabel* label = NULL;

    const std::string& propName = labelParam.first;
    const PropValueTable* pvTable = groupManager_->getPropValueTable(propName);
    if (pvTable)
    {
        label = new StringGroupLabel(labelParam.second, *pvTable);
    }
    else
    {
        LOG(ERROR) << "group index file is not loaded for group property " << propName;
    }

    return label;
}

NS_FACETED_END
