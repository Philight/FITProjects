__author__ = 'xlainh00'

import tweepy
import re
import os
import urllib.request

## ---------------------------------------- MAIN ------------------------------------------
if __name__ == '__main__':
    # Identification for twitter API
    consumer_key = "wfiTVwGQEbqe6ZOkjdF6eHORt"
    consumer_secret = "uHYFTwCNUIp6bqGfGdYQ4LfXMfb3ErGqYvb4fsEN6YLXLndY7t"
    access_token = "3179251144-j5wySeN3LwI4XqO0b5gRplOw7zhl5Rcjgj7skBZ"
    access_token_secret = "sDvDB4ZhnNK0z2Jn5Uu8RRwozKSzP7HUTQQZ9FEnyumhh"

    print("Downloading Tweets from @LAHWF...")
    # OAuth process for twitter
    auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)
    api = tweepy.API(auth)
    username = 'LAHWF'
    number_of_tweets = 200

    # Download users timeline with tweets
    tweets = api.user_timeline(screen_name=username, count = number_of_tweets)

    # Save tweets in a list
    tweetsList = [[username, tweet.id_str, tweet.created_at, tweet.text.encode("utf-8")] for tweet in tweets]

    # Write into file 'Tweets'
    fp = open('Tweets', 'w')
    print("Author| Tweet Id\t\t|Date Posted\t\t|Content", file=fp)
    print("-------------------------------------------------------", file=fp)

    for eachTweet in tweetsList:
        # Format tweets list
        print('|'.join(str(eachData) for eachData in eachTweet), file=fp)

        # Find URLs mentioned in tweet
        links = re.findall(r'(https?://[^\s\'\"]+)', str(eachTweet[3]))
        if links:
            if len(links) == 1: urlword = "URL"
            else: urlword = "URLs"
            print(str(len(links)), urlword, "found. Source codes will be saved in separate files.")
            for eachLink in links:
                print(eachLink)
                usock = urllib.request.urlopen(str(eachLink))
                data = usock.read()
                usock.close()

                # Save HTML source code to file with {Tweet Id-linkaddress} name
                filename = eachTweet[1] + "-" + re.search('https?://t.co/(.*)', eachLink).group(1)
                if not os.path.exists("URL"): # Create dir URL
                    os.makedirs("URL")
                with open("URL/"+filename, 'w') as htmlfile:
                    print(data, file=htmlfile)
    fp.close()
    print("Finished. Tweets saved in 'Tweets'")
