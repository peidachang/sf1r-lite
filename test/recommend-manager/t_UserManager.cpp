///
/// @file t_UserManager.cpp
/// @brief test UserManager in User operations
/// @author Jun Jiang
/// @date Created 2011-04-18
///

#include <util/ustring/UString.h>
#include <recommend-manager/UserManager.h>

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

using namespace std;
using namespace boost;
using namespace sf1r;

namespace bfs = boost::filesystem;

namespace
{
const izenelib::util::UString::EncodingType ENCODING_TYPE = izenelib::util::UString::UTF_8;
const char* TEST_DIR_STR = "recommend_test";
const char* USER_DB_STR = "user.db";
}

void checkUser(const User& user1, const User& user2)
{
    BOOST_CHECK_EQUAL(user1.id_, user2.id_);
    BOOST_CHECK_EQUAL(user1.idStr_, user2.idStr_);

    User::PropValueMap::const_iterator it1 = user1.propValueMap_.begin();
    User::PropValueMap::const_iterator it2 = user2.propValueMap_.begin();
    for (; it1 != user1.propValueMap_.end() && it2 != user2.propValueMap_.end(); ++it1, ++it2)
    {
        BOOST_CHECK_EQUAL(it1->second, it2->second);
    }

    BOOST_CHECK(it1 == user1.propValueMap_.end());
    BOOST_CHECK(it2 == user2.propValueMap_.end());
}

void checkUserManager(const vector<User>& userVec, UserManager& userManager)
{
    BOOST_CHECK_EQUAL(userManager.userNum(), userVec.size());

    User user2;
    for (vector<User>::const_iterator it = userVec.begin();
        it != userVec.end(); ++it)
    {
        BOOST_CHECK(userManager.getUser(it->id_, user2));
        checkUser(*it, user2);
    }
}

void iterateUserManager(const vector<User>& userVec, UserManager& userManager)
{
    BOOST_CHECK_EQUAL(userManager.userNum(), userVec.size());

    int iterNum = 0;
    for (UserManager::SDBIterator userIt = userManager.begin();
        userIt != userManager.end(); ++userIt)
    {
        const User& user = userIt->second;
        BOOST_CHECK_EQUAL(userIt->first, user.id_);

        bool isFind = false;
        for (vector<User>::const_iterator it = userVec.begin();
                it != userVec.end(); ++it)
        {
            if (it->id_ == user.id_)
            {
                checkUser(user, *it);
                isFind = true;
                break;
            }
        }
        BOOST_CHECK(isFind);
        ++iterNum;
    }

    BOOST_TEST_MESSAGE("iterNum: " << iterNum);
    BOOST_CHECK_EQUAL(iterNum, userManager.userNum());
}

BOOST_AUTO_TEST_SUITE(UserManagerTest)

BOOST_AUTO_TEST_CASE(checkUser)
{
    bfs::path userPath(bfs::path(TEST_DIR_STR) / USER_DB_STR);
    boost::filesystem::remove(userPath);
    bfs::create_directories(TEST_DIR_STR);

    vector<User> userVec;
    userVec.push_back(User());
    User& user1 = userVec.back();
    user1.id_ = 1;
    user1.idStr_ = "aaa_1";
    user1.propValueMap_["gender"].assign("男", ENCODING_TYPE);
    user1.propValueMap_["age"].assign("20", ENCODING_TYPE);
    user1.propValueMap_["job"].assign("student", ENCODING_TYPE);
    user1.propValueMap_["interest"].assign("reading", ENCODING_TYPE);

    userVec.push_back(User());
    User& user2 = userVec.back();
    user2.id_ = 51;
    user2.idStr_ = "aaa_51";
    user2.propValueMap_["gender"].assign("女", ENCODING_TYPE);
    user2.propValueMap_["age"].assign("30", ENCODING_TYPE);
    user2.propValueMap_["job"].assign("finance", ENCODING_TYPE);
    user2.propValueMap_["interest"].assign("movie", ENCODING_TYPE);

    {
        BOOST_TEST_MESSAGE("add user...");
        UserManager userManager(userPath.string());
        for (vector<User>::const_iterator it = userVec.begin();
            it != userVec.end(); ++it)
        {
            BOOST_CHECK(userManager.addUser(*it));
        }

        checkUserManager(userVec, userManager);
        iterateUserManager(userVec, userManager);

        userManager.flush();
    }

    {
        BOOST_TEST_MESSAGE("update user...");
        UserManager userManager(userPath.string());

        checkUserManager(userVec, userManager);

        User& user2 = userVec.back();
        user2.propValueMap_["gender"].assign("femal", ENCODING_TYPE);
        user2.propValueMap_["age"].assign("33", ENCODING_TYPE);
        user2.propValueMap_["job"].assign("lawyer", ENCODING_TYPE);
        user2.propValueMap_["interest"].assign("shopping", ENCODING_TYPE);
        user2.propValueMap_["born"].assign("上海", ENCODING_TYPE);

        BOOST_CHECK(userManager.updateUser(user2));
        checkUserManager(userVec, userManager);
        iterateUserManager(userVec, userManager);

        BOOST_TEST_MESSAGE("remove user...");
        User& user1 = userVec.front();
        BOOST_CHECK(userManager.removeUser(user1.id_));
        userVec.erase(userVec.begin());
        checkUserManager(userVec, userManager);
        iterateUserManager(userVec, userManager);

        userManager.flush();
    }

    {
        BOOST_TEST_MESSAGE("empty user...");
        UserManager userManager(userPath.string());

        checkUserManager(userVec, userManager);
        iterateUserManager(userVec, userManager);

        User& user2 = userVec.front();
        BOOST_CHECK(userManager.removeUser(user2.id_));
        userVec.erase(userVec.begin());
        checkUserManager(userVec, userManager);
        iterateUserManager(userVec, userManager);

        userManager.flush();
    }
}

BOOST_AUTO_TEST_SUITE_END() 
