/**
 * @file ProductTokenizer.h
 * @brief tokenize product title and query
 */

#ifndef PRODUCT_TOKENIZER_H
#define PRODUCT_TOKENIZER_H

#include <util/ustring/UString.h>
#include <string>
#include <list>

namespace sf1r
{

struct ProductTokenParam
{
    const std::string& query;
    const bool isRefineResult;

    typedef std::pair<izenelib::util::UString, double> TokenScore;
    typedef std::list<TokenScore> TokenScoreList;
    typedef TokenScoreList::const_iterator TokenScoreListIter;

    TokenScoreList majorTokens;
    TokenScoreList minorTokens;

    izenelib::util::UString refinedResult;

    double rankBoundary;

    // use Fuzzy Threshold 
    bool useFuzzyThreshold;
    float fuzzyThreshold;
    float tokensThreshold;

    // use Privilege Query
    bool usePrivilegeQuery;
    std::string privilegeQuery;
    float privilegeWeight;

    ProductTokenParam(const std::string& queryParam, bool isRefineParam)
            : query(queryParam)
            , isRefineResult(isRefineParam)
            , rankBoundary(0.0F)
            , useFuzzyThreshold(false)
            , fuzzyThreshold(0.5F)
            , tokensThreshold(0.5F)
            , usePrivilegeQuery(false)
            , privilegeWeight(0.0F)
    {}
};

class ProductTokenizer
{
public:
    virtual ~ProductTokenizer() {}

    virtual void tokenize(ProductTokenParam& param) = 0;

    virtual double sumQueryScore(const std::string& query)
    {
        return 0;
    }

protected:
    bool isProductType_(const izenelib::util::UString& str);

    bool getBigramTokens_(
        const std::list<std::string>& input,
        std::list<std::pair<izenelib::util::UString, double> >& tokens,
        double score = 1.0);

    bool getBigramTokens_(
        const std::list<izenelib::util::UString>& input,
        std::list<std::pair<izenelib::util::UString, double> >& tokens,
        double score = 1.0);

    void doBigram_(
        const izenelib::util::UString& pattern,
        std::list<std::pair<izenelib::util::UString, double> >& tokens,
        double score);

protected:
    static const izenelib::util::UString::CharT SPACE_UCHAR = ' ';
};

}

#endif // PRODUCT_TOKENIZER_H
