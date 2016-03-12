TorrentSync
===========

The aim of this software is to provide an opensource alternative to BTSync that
is more usable than git-annex.

The software must work without a centralized server and is currently the Torrent
DHT protocol is being implemented for discovering members of the same shared
folder system.

The main aim is to be able to run this on very low end hardware without
performance issues. All the network code is asynchronous and doesn't require any
additional thread to be efficient, even though more thread can be started and
made to enter the boost::asio runloop.

The Current target:

Implementing working DHT as of BEP 005
Later on BEP 32,42

Status
===========

Needed components

Torrent DHT implementation to find nodes
  * Routing table
    partially done, missing bucket refilling, ..
  * Protocol implementation
    partially done
  * Bootstrap via cache
  * Rendezvouses mechanism
  * Decode messages in place with boost::range.

Transport system
  * Metadata synchronization protocol
  * File synchronization protocol
  * File synchronization algorithm
  * Conflict resolution

Files
  * Checksum management
    - Merkle tree
  * Checksum builder and reader
    - libnettle?
  * File changes notification systems interface
    - inotify, ..

Glue stuff together
  * Main app

Additional stuff
  * NAT, UPNP
  * Hole punching systems
  * bandwidth reduction by checksum hash table checking if we have the data already

Ongoing
=======

Torrent DHT implementation to find nodes
  * implement announce behaviour and messages

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
