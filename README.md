Current target:

Implementing working DHT as of BEP 005
Later on BEP 32,42

TorrentSync
===========

Needed components

Torrent DHT implementation to find nodes
  * Routing table
    partially done, missing bucket refilling, ..
  * Protocol implementation
    partially done
  * Bootstrap via cache
  * Randeveouzes mechanism

Transport system
  * Metadata synchronization protocol
  * File synchronization protocol
  * File synchronization algorithm
  * Conflict resolution

Files
  * Checksum management
  * Checksum builder and reader
  * File changes notification systems interface

Glue stuff together
  * Main app

Additional stuff
  * NAT, UPNP
  * Hole punching systems
  * bandwidth reduction by checksum hash table checking if we have the data already

Ongoing
=======

Torrent DHT implementation to find nodes
  * Messages implementation
    announce

Done
====

Automation
  * building scripts
  * testing scripts

Torrent DHT implementation to find nodes
  * Routing Table internal structure
  * Bootstrap via fixed nodes
  * Messages implementation
    done:
        find_node
        ping
        get_peers

Glue stuff together
  * Minimal app for development testing

Needed build Packages
===============

* Ubuntu
    * cmake
    * gcov,lcov (for unit test coverage)
    * libboost1.48-all-dev
