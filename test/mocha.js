var assert = require('assert');
var mt = require('..');
var JZZ = require('jzz');

describe('MIDI messages', function() {
  it('jzz', function() {
    JZZ().and(function(){ console.log(this.info()); });
  });
  it('virtual-midi-in', function() {
    var port = mt.MidiSrc('Virtual MIDI-In');
    port.connect();
    JZZ().and(function(){ console.log(this.info()); });
  });
});
