const mt = require('.');
console.log('mt:', mt);
console.log('mt.MidiSrc:', mt.MidiSrc);
console.log('mt.MidiDst:', mt.MidiDst);

var src = new mt.MidiSrc();
console.log('new MidiSrc():', src, src.echo, src.echo('src'));

var sss = mt.MidiSrc();
console.log('MidiSrc():', sss, sss.echo, sss.echo('sss'));

var dst = new mt.MidiDst();
console.log('new MidiDst():', dst, dst.echo, dst.echo('dst'));

var ddd = mt.MidiDst();
console.log('MidiDst():', ddd, ddd.echo, ddd.echo('ddd'));
