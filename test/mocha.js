var assert = require('assert');
var mt = require('..');
var JZZ = require('jzz');

describe('MIDI messages', function() {
  it('jzz', function(done) {
    JZZ().and(function() {
      console.log(this.info());
      done();
    });
  });
  it('virtual-midi-in', function(done) {
    var port = mt.MidiSrc('Virtual MIDI-In');
    port.connect();
    JZZ().refresh().and(function() {
      console.log(this.info());
      port.disconnect();
      done();
    });
  });
  it('virtual-midi-out', function(done) {
    var port = mt.MidiDst('Virtual MIDI-Out');
    port.connect();
    JZZ().refresh().and(function() {
      console.log(this.info());
      port.disconnect();
      done();
    });
  });
  it('virtual-midi-in-busy', function(done) {
    var port = mt.MidiSrc('Virtual MIDI-In Busy');
    port.connect();
    assert.equal(port.busy, false);
    port.busy = 'something';
    assert.equal(port.busy, true);
    port.busy = '';
    assert.equal(port.busy, false);
    port.disconnect();
    done();
  });
  it('virtual-midi-out-busy', function(done) {
    var port = mt.MidiDst('Virtual MIDI-Out Busy');
    port.connect();
    assert.equal(port.busy, false);
    port.busy = 1;
    assert.equal(port.busy, true);
    port.busy = 0;
    assert.equal(port.busy, false);
    port.disconnect();
    done();
  });
});
