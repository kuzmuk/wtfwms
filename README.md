[WTF with my server](http://wtfwms.com)
==================

Extraordinary tool for researching web application performance issues.

This is a source code for client that should be installed on servers. It fetches all needed metrics and sends them to [wtfwms.com](http://wtfwms.com) dashboard.

You can find prebuilt binaries for some Centos and Ubuntu distros or build own package for any other linux platform.

(c) Alex Kuzmuk, Andrey Bistriy, Taras Tarasov

How It Works
============

You install a special tool that runs in the background on your server and set up only a few options in a configuration file.

WTFwms daemon constantly tracks active processes on your servers. One it notices that there are too many active workers it takes a snapshot of the most important system metrics and posts them on your dashboard where you can monitor and analyze it.

WTFwms tracks CPU load, I/O wait time, memory and swam usage, list of TOP 10 active processes, TOP 5 slowest MySQL queries that are running now on each of your shared, open files and network connection of each active worker. It even traces all system calls of your workers so you can know exactly what they were doing at that moment. All of this works without any additional code in your application.


Features
========

1. Zero Code Changes. There are no application code changes needed. WTFwms gathers all needed information without additional help from your code.

2. Peak Load System Metrics. WTFwms takes snapshots of the most critical system metrics in peak load moments - CPU Load, IO Wait, Memory Usage, Swap Usage, Load Average and more.

3. Shards. WTFwms can monitor as many MySQL shards as you have to allow you to see the whole picture.

4. Trace System Calls. WTFwms traces system calls of your backend workers. You can see what is going on inside of slow processes.

5. Open Files and TCP Connections. WTFwms shows all open files and TCP Connections per each backend worker. You can see what web-services are in use and what files are being accessed now.

6. Hijack Read and Write Operations. WTFwms can hijack data from read/write system calls. This allows you to see exact data that was read/write from/to any file or socket.

7. Top MySQL queries. WTFwms captures most slow SQL queries that were running in peak load moments to allow you to analyze and optimize them.

8. Mask SQL Queries. Enable Mask SQL feature and we will not transmit and store any sensitive values from your SQL queries. All values will be replaced with the '%' character.

9. Minimal Performance Overhead. WTFwms is designed to do nothing while your server is running ok. Daemon triggers only when the server is overloaded. In such moments, it gathers important metrics and will send a report to our server to be analyzed.

10. Worker's URL. Are you using Apache as a backend server? If so, you can see a virtual host name and URL that each worker was handling in peak load moments.

11. Designed for High Load. High Load environments have different requirements for tools. Having to deal with thousands of requests per second and thousands of open network connections requires different approaches to complete even trivial tasks. WTFwms is proven to be ready to work in these environments.

12. Native Code for Best Performance. WTFwms client is written using C++ and is compiled for most popular server operation systems. It does not use any other binaries or scripts to gather all metrics. Nothing else compares in speed.


License
=======

You can use and modify this daemon as you want. You should keep copyrights, author's information and link to wtfwms.com. You can built package for any platform. If you will build your own dashboard you must provide a link to [wtfwms.com](http://wtfwms.com) site.
