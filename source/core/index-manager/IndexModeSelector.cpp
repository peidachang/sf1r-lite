#include "IndexModeSelector.h"
#include "IndexManager.h"
using namespace sf1r;

/// @brief The threshold is set to the scd_file_size/exist_doc_num, if smaller or equal than this threshold then realtime mode will turn on.
IndexModeSelector::IndexModeSelector(const boost::shared_ptr<IndexManager>& index_manager, double threshold, long max_realtime_msize)
: index_manager_( index_manager), threshold_(threshold), max_realtime_msize_(max_realtime_msize)
{
}

void IndexModeSelector::TrySetIndexMode(long scd_file_size)
{
    uint32_t exist_docs = index_manager_->getIndexReader()->numDocs();
    if(exist_docs==0)
    {
        index_manager_->setIndexMode("default");
        return;
    }

    long mill_size = scd_file_size/1024/1024;
    if(mill_size>max_realtime_msize_)
    {
        index_manager_->setIndexMode("default");
        return ;
    }
    double rate = (double)mill_size/exist_docs;
    if(rate<=threshold_)
    {
        std::cout<<"index mode : realtime"<<std::endl;
        index_manager_->setIndexMode("realtime");
    }
    else
    {
        index_manager_->setIndexMode("default");
    }

}

void IndexModeSelector::TryCommit()
{
    if(! index_manager_->isRealTime())
    {
        index_manager_->flush();
    }
    else
    {
        ///Only flush BTreeIndexer
        ///It might be removed for new IndexSDB
        index_manager_->getBTreeIndexer()->flush();
    }
}

void IndexModeSelector::ForceCommit()
{
    if(index_manager_->isRealTime())
    {
        index_manager_->flush();
    }
}
