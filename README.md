# midi-test

[![npm](https://img.shields.io/npm/v/midi-test.svg)](https://www.npmjs.com/package/midi-test)
[![npm](https://img.shields.io/npm/dt/midi-test.svg)](https://www.npmjs.com/package/midi-test)
[![Build Status](https://travis-ci.com/jazz-soft/midi-test.svg?branch=master)](https://travis-ci.com/jazz-soft/midi-test)

We would really appreciate your [**support**](https://jazz-soft.net/donate)!

## Virtual MIDI ports for testing MIDI applications

See also: [**web-midi-test**](https://github.com/jazz-soft/web-midi-test)

Install: `npm install midi-test --save-dev`

Manual rebuild (if needed): `node-gyp rebuild`

## API
### MIDI Source (Virtual MIDI-In)

    var MT = require('midi-test');
    var port = new MT.MidiSrc('VIRTUAL MIDI-In');
    port.connect();
    port.emit([0x90, 0x40, 0x7f]);
    //...
    port.disconnect();

### MIDI Destination (Virtual MIDI-Out)

    var MT = require('midi-test');
    var port = new MT.MidiDst('VIRTUAL MIDI-Out');
    port.receive = function(msg) { console.log('received:', msg); };
    port.connect();
    //...
    port.disconnect();

### TypeScript

    import * as MT from 'midi-test'
    //...

## Developer notes

**midi-test** currently works in **Linux** and **MacOS**.  
**Windows** version may appear some day in the future, but it is not in the immediate plans.

This package requires **Node.js v10.6.0 or later**,
and the full stack of [**node-gyp**](https://www.npmjs.com/package/node-gyp) build tools.  
Linux build also requires the ALSA libraries: `apt-get install libasound2-dev`

Since the Linux version needs special permissions to access the computer's sound system,  
it may not work properly in virtual Linux environments such as the one at [**travis-ci**](https://travis-ci.org).

To make your test work at **travis-ci**, run it in osx.  
To do so, add the following lines in your *.travis.yml*:

    os:
      - osx


