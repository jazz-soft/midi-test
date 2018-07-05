const mt = require('.');
console.log('mt:', mt);
console.log('mt.MidiSrc:', mt.MidiSrc);
console.log('mt.MidiDst:', mt.MidiDst);

var src1 = new mt.MidiSrc();
var src2 = mt.MidiSrc('test!!! - something longer than 16');

console.log('src1:', src1);
console.log('src1.name:', src1.name);
console.log('src1.connected:', src1.connected);
console.log('src1.emit([1, 2, 3]):', src1.emit([1, 2, 3]));
console.log('connecting...');
var res = src1.connect();
console.log('result:', res);
console.log('src1.connected:', src1.connected);
console.log('src1.emit([1, 2, 3]):', src1.emit([1, 2, 3]));

console.log('disconnecting...');
res = src1.disconnect();
console.log('result:', res);
console.log('src1.connected:', src1.connected);
console.log('src1.emit([1, 2, 3]):', src1.emit([1, 2, 3]));

console.log('src2:', src2);
console.log('src2.name:', src2.name);
