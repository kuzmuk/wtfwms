wtfwms
======

Extraordinary tool for researching web application performance issues



How It Works
============

You install a special tool that runs in the background on your server and set up only a few options in a configuration file.

WTFwms daemon constantly tracks active processes on your servers. One it notices that there are too many active workers it takes a snapshot of the most important system metrics and posts them on your dashboard where you can monitor and analyze it.

WTFwms tracks CPU load, I/O wait time, memory and swam usage, list of TOP 10 active processes, TOP 5 slowest MySQL queries that are running now on each of your shared, open files and network connection of each active worker. It even traces all system calls of your workers so you can know exactly what they were doing at that moment. All of this works without any additional code in your application.

