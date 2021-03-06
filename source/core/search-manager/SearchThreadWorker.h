/**
 * @file SearchThreadWorker.h
 * @brief the worker to implement search logic in one thread.
 * @author Jun Jiang
 * @date Created 2013-01-09
 */

#ifndef SF1R_SEARCH_THREAD_WORKER_H
#define SF1R_SEARCH_THREAD_WORKER_H

#include <common/inttypes.h>
#include <map>
#include <boost/shared_ptr.hpp>

namespace sf1r
{
class InvertedIndexManager;
class DocumentManager;
class SearchManagerPreProcessor;
class CustomRankManager;
class KeywordSearchActionItem;
class DocumentIterator;
class ScoreDocEvaluator;
class PropSharedLockSet;
class QueryBuilder;
class IndexBundleConfiguration;
class RankingManager;
struct SearchThreadParam;

namespace faceted
{
class GroupFilter;
class GroupFilterBuilder;
}

class SearchThreadWorker
{
public:
    SearchThreadWorker(
        const IndexBundleConfiguration& config,
        const boost::shared_ptr<DocumentManager>& documentManager,
        const boost::shared_ptr<InvertedIndexManager>& indexManager,
        const boost::shared_ptr<RankingManager>& rankingManager,
        SearchManagerPreProcessor& preprocessor,
        QueryBuilder& queryBuilder);

    void setGroupFilterBuilder(const faceted::GroupFilterBuilder* builder);

    void setCustomRankManager(CustomRankManager* customRankManager);

    bool search(SearchThreadParam& param);

private:
    /**
     * combine the @p originDocIterator with the customized doc iterator.
     * @return the combined doc iterator instance, it would be just
     *         @p originDocIterator if no customized doc iterator is created.
     */
    DocumentIterator* combineCustomDocIterator_(
        const KeywordSearchActionItem& actionItem,
        DocumentIterator* originDocIterator);

    bool doSearch_(
        SearchThreadParam& param,
        DocumentIterator& docIterator,
        faceted::GroupFilter* groupFilter,
        ScoreDocEvaluator& scoreDocEvaluator,
        PropSharedLockSet& propSharedLockSet);

private:
    const IndexBundleConfiguration& config_;

    boost::shared_ptr<DocumentManager> documentManagerPtr_;
    boost::shared_ptr<InvertedIndexManager> indexManagerPtr_;
    boost::shared_ptr<RankingManager> rankingManagerPtr_;

    SearchManagerPreProcessor& preprocessor_;

    QueryBuilder& queryBuilder_;

    const faceted::GroupFilterBuilder* groupFilterBuilder_;

    CustomRankManager* customRankManager_;

    std::map<propertyid_t, float> propertyWeightMap_;
};

} // namespace sf1r

#endif // SF1R_SEARCH_THREAD_WORKER_H
