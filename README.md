# RFCCAT

[![Build Status](https://travis-ci.org/Robiathin/rfccat.svg?branch=master)](https://travis-ci.org/Robiathin/rfccat)

A small, simple tool for reading RFCs from the command line. Uses libcurl to fetch RFC pages and displays them using a pager.

## Usage

Simply specify an RFC number:

```sh
rfccat 2616
```

## Installation

Download the latest release from [github.com/Robiathin/rfccat/releases](https://github.com/Robiathin/rfccat/releases)
Extract the archive and change directory into it.

Then run:

```sh
make
sudo make install
```

## Dependencies

- libcurl
