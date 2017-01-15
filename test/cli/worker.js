
var assert = require('assert');
var fs = require('fs');
var vm = require('vm');
var worker = {};

var includeInThisContext = function(path) {
    var code = fs.readFileSync(path);
    vm.runInThisContext(code, path);
};

global.importScripts = function(path) {
    includeInThisContext('src/worker/'+path);
}

function Blob(blob) {
  this.size = blob.length;
  this.length = blob.length;
  this.slice = function(a,b) {
    var data = blob.slice(a,b);
    var b = new Blob(data);
    //console.log(a, b, data.length, data.slice(0,64));
    //console.log(new Error().stack);
    return b;
  }
  this.asArrayBuffer = function() {
    var buf = new ArrayBuffer(blob.length);
    var arr = new Uint8Array(buf);
    for (var i=0; i<blob.length; i++)
      arr[i] = blob[i].charCodeAt(0);
    return arr;
  }
}

global.XMLHttpRequest = function() {
    this.open = function(a,b,c) {
        if (this.responseType == 'json') {
            var txt = fs.readFileSync('src/worker/'+b);
            this.response = JSON.parse(txt);
        } else if (this.responseType == 'blob') {
            var data = fs.readFileSync('src/worker/'+b, {encoding:'binary'});
            //var buf = new ArrayBuffer(data.length);
            //var blob = new Uint8Array(buf);
            //blob.set(data);
            this.response = new Blob(data);
        }
    }
    this.send = function() { }
}

global.FileReaderSync = function() {
  this.readAsArrayBuffer = function(blob) {
    return blob.asArrayBuffer();
  }
}

global.onmessage = null;
global.postMessage = null;

includeInThisContext("src/worker/workermain.js");

function compile(tool, code, platform, callback, outlen, nlines, nerrors) {
    global.postMessage = function(msg) {
        if (msg.errors && msg.errors.length) {
          console.log(msg.errors);
          assert.equal(nerrors, msg.errors.length, "errors");
        } else {
          assert.equal(nerrors||0, 0, "errors");
          assert.equal(msg.output.length, outlen, "output binary");
          assert.equal(msg.lines.length, nlines, "listing lines");
        }
        callback(null, msg);
    };
    global.onmessage({
        data:{code:code, platform:platform, tool:tool}
    });
}

describe('Worker', function() {
  it('should assemble DASM', function(done) {
    compile('dasm', '\tprocessor 6502\n\torg $f000\nfoo lda #0\n', 'vcs', done, 2, 1);
  });
  it('should assemble ACME', function(done) {
    compile('acme', 'foo: lda #0\n', 'vcs', done, 2, 0); // TODO
  });
  it('should compile PLASMA', function(done) {
    compile('plasm', 'word x = 0', 'apple2', done, 5, 0);
  });
  it('should compile CC65', function(done) {
    compile('cc65', '#include <stdio.h>\nint main() {\nint x=1;\nprintf("%d",x);\nreturn x+2;\n}', 'apple2', done, 2947, 4);
  });
  it('should NOT assemble Z80ASM', function(done) {
    compile('z80asm', 'ddwiuweq', 'none', done, 0, 0, 1);
  });
  it('should assemble Z80ASM', function(done) {
    compile('z80asm', '\tMODULE test\n\tXREF _puts\n\tld	hl,$0000\n\tret\n', 'none', done, 4, 2, 0);
  });
  it('should NOT compile SDCC', function(done) {
    compile('sdcc', 'foobar', 'none', done, 0, 0, 1);
  });
  it('should compile SDCC', function(done) {
    compile('sdcc', 'int main(int argc) {\nint x=1; int y=2;\nreturn x+y+argc;\n}', 'none', done, 16, 2, 0);
  });
});