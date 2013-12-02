TorrentSync
===========

Needed components

Torrent DHT implementation to find nodes
  * Routing table
  * Protocol implementation
  * Bootstrap modes

Transport system
  * Metadata synchronization protocol
  * File synchronization protocol
  * File synchronization algorithm
  * Conflict reslution

Files
  * Checksum management
  * Checksum builder and reader
  * File changes notification systems interface

Glue stuff together
  * Minimal app for development testing



Additional stuff
  * NAT, UPNP
  * Hole punching systems
  * bandwith reduction by checksum hashtable checking if we have the data already
  

Done:

Automation
  * building scripts
  * testing scripts
