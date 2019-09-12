// double, double => rotation matrix
// require https://cdn.rawgit.com/humbletim/glm-js/31fd034b/build/glm-js.min.js


var arcballLib = {
    arcball: function(oldmouse, newmouse) {
        let oldMouseNew = [0, 0, 0];
        let newMouseNew = [0, 0, 0];

        let testNorm = this.twonorm(oldmouse);
        if (testNorm > 1) {
            oldMouseNew[0] = oldmouse[0] / testNorm;
            oldMouseNew[1] = oldmouse[1] / testNorm;
        } else {
            oldMouseNew[0] = oldmouse[0];
            oldMouseNew[1] = oldmouse[1];
        }

        let testNorm2 = this.twonorm(newmouse);
        if (testNorm2 > 1) {
            newMouseNew[0] = newmouse[0] / testNorm2;
            newMouseNew[1] = newmouse[1] / testNorm2;
        } else {
            newMouseNew[0] = newmouse[0];
            newMouseNew[1] = newmouse[1];
        }

        var z1squared = 1 - oldMouseNew[1] ** 2 - oldMouseNew[0] ** 2;
        var z2squared = 1 - newMouseNew[1] ** 2 - newMouseNew[0] ** 2;

        oldMouseNew[2] = Math.sqrt(Math.max(z1squared,0));
        newMouseNew[2] = Math.sqrt(Math.max(z2squared,0));

        let oldMouseVec = glm.vec3(...oldMouseNew);
        let newMouseVec = glm.vec3(...newMouseNew);
        let axis = glm.normalize(glm.cross(oldMouseVec, newMouseVec));

        let dot=glm.dot(oldMouseVec, newMouseVec);
        if(dot > 1) dot=1;
        else if(dot < -1) dot=-1;
        let angle = Math.acos(dot);

        return [angle, axis]

    }, 

    twonorm: function(v) {
        let normSq = 0
        for (let i = 0; i < v.length; i++) {
            normSq += v[i] ** 2;
        }
        return Math.sqrt(normSq);
    }
}
