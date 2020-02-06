function dec2bin(dec) {
    return (dec >>> 0).toString(2);
}

function reduce(t, m) {
    let a = t;
    const n = dec2bin(t).length;

    for (let i = 0; i < n; i++) {
        if ((a & 1) === 1) {
            a += m;
        }

        a = a >> 1;
    }
    if (a >= m) a -= m;
    return a;
}

console.log(reduce(17, 23));