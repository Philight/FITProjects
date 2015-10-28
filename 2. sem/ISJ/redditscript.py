__author__ = 'xlainh00'
##
## Skript uloziava udaje do suboru 'RedditPosts'
##

import praw
import os
from datetime import datetime

## Convert Unix time to real time
def convert_time(unixFormat):
    return (datetime.fromtimestamp(int(unixFormat)))

## if check = 0, download all posts, elif check = 1, check for new posts
def RunScript(check):
    for submission in subreddit.get_new(limit = 200):
        submissionposted = convert_time(submission.created_utc)
        if check:
            if submissionposted < lastRunTime:  ## exit loop if older post
                break

        print("########################################################################################################################", file=fp)
        print("########################################################################################################################", file=fp)
        print ("Title: ", submission.title, file=fp)
        print ("Author: ", submission.author, file=fp)

        category = submission.link_flair_text
        if not category:
            category = "None"
        print ("Category: ", category, file=fp)
        print ("Submitted on: ", submissionposted, file=fp)
        print ("Score: ", submission.score, file=fp)
        print ("Link: ", submission.short_link, file=fp)
        print ("---------------------------------", file=fp)
        print (submission.selftext, "\n", file=fp)
        print ("---------------------COMMENTS(", submission.num_comments, ")---------------------", file=fp)

        for eachComment in submission.comments:
            if not hasattr(eachComment, 'body'):
                continue
            print(eachComment.body, file=fp)
            print("Upvotes: ", eachComment.ups, file=fp)

            commentTime = convert_time(eachComment.created_utc)
            yearDiff = currentTime.year - commentTime.year
            monthDiff = currentTime.month - commentTime.month
            dayDiff = currentTime.day - commentTime.day
            hourDiff = currentTime.hour - commentTime.hour
            minuteDiff = currentTime.minute - commentTime.minute
            secondDiff = currentTime.second - commentTime.second

            if yearDiff > 1:
                commentPosted = "%d years ago" %yearDiff
            elif yearDiff == 1:
                commentPosted = "1 year ago"
            else:
                if monthDiff > 1:
                    commentPosted = "%d months ago" % monthDiff
                elif monthDiff == 1:
                    commentPosted = "1 month ago"
                else:
                    if dayDiff > 1:
                        commentPosted = "%d days ago" % dayDiff
                    elif dayDiff == 1:
                        commentPosted = "1 day ago"
                    else:
                        if hourDiff > 1:
                            commentPosted = "%d hours ago" % hourDiff
                        elif hourDiff == 1:
                            commentPosted = "1 hour ago"
                        else:
                            if minuteDiff > 1:
                                commentPosted = "%d minutes ago" % minuteDiff
                            elif minuteDiff == 1:
                                commentPosted = "1 minute ago"
                            else:
                                if secondDiff > 1:
                                    commentPosted = "%d seconds ago" % secondDiff
                                elif secondDiff == 1:
                                    commentPosted = "1 second ago"
                                else:
                                    commentPosted = "now"
            print(commentPosted, "\n", file=fp)
            print("-------------------------\n", file=fp)



## ---------------------------------------- MAIN ------------------------------------------
if __name__ == '__main__':
    currentTime = datetime.now().replace(microsecond=0) ## Current time
    timeFormat = '%Y-%m-%d %H:%M:%S'

    r = praw.Reddit(user_agent = "User Agent: xlainh00@fit.vutbr.cz Save Reddit Posts Script 1.0 by Philai") ## User information for PRAW API
    subreddit = r.get_subreddit("IAmA")

    filename = 'RedditPosts'

## OPTIONS - Download new posts or all posts
    if os.path.isfile(filename) == 1 and os.path.getsize(filename) > 0: ## File exists and isnt empty
        statbuf = os.stat(filename)
        lastRunTime = convert_time(statbuf.st_mtime) ## last runtime

        validInput = 0
        while not validInput:
            userInput = input('Do you want to check for new posts? ( y/n )')
            if userInput == 'y' or userInput == 'Y':
                print ("Checking for new posts...")
                validInput = 1
                with open(filename, 'r', encoding='utf-8') as original: olddata = original.read()    ## open and save old file
                with open(filename, 'w', encoding='utf-8') as fp: RunScript(check=1)   # Rewrite with new data
                with open(filename, 'r', encoding='utf-8') as rewritten: newdata = rewritten.read()   ## Save new data
                with open(filename, 'w', encoding='utf-8') as modified: modified.write(newdata + "\n" + olddata) ## Prepend new data infront of old data

            elif userInput == 'n' or userInput == 'N':
                print ("Running script...")
                fp = open(filename, 'w', encoding='utf-8')
                RunScript(check=0)
                validInput = 1
                fp.close()
    else:
    ## First time script run/file doesnt exist
        print("File not found. Creating file - RedditPosts")
        print ("Running script...")
        with open(filename, 'w', encoding='utf-8') as fp:
            statbuf = os.stat(filename)
            lastRunTime = convert_time(statbuf.st_mtime) ## last runtime
            RunScript(check=0)

    print("Finished. Posts saved in 'RedditPosts'")