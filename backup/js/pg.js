adjacencyMatrix = $M([
    [0,0,0,0,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,0,0,0,0],
    [0,1,0,0,0,0,0,0,0,0,0],
    [1,1,0,0,0,0,0,0,0,0,0],
    [0,1,0,1,0,1,0,0,0,0,0],
    [0,1,0,0,1,0,0,0,0,0,0],
    [0,1,0,0,1,0,0,0,0,0,0],
    [0,1,0,0,1,0,0,0,0,0,0],
    [0,1,0,0,1,0,0,0,0,0,0],
    [0,0,0,0,1,0,0,0,0,0,0],
    [0,0,0,0,1,0,0,0,0,0,0]
]);

Matrix.prototype.row_stochastic = function(damping_factor) {
 
    var row_length = this.elements[0].length;
    var d = (1 - damping_factor) / row_length;
 
    var row_total = [];
 
    for (var x = 0; x < row_length; x++) {
        row_total.push(0);
        for (y = 0; y < row_length; y++) {
            row_total[x] += this.elements[x][y];
        }
    }
 
    var a1 = this.elements.clone();
 
    for (var x = 0; x < row_length; x++) {
        for (var y = 0; y < row_length; y++) {
            if (row_total[x] > 0) {
                a1[x][y] = a1[x][y]/row_total[x] + d;
            }
            else {
                a1[x][y] = (1/row_length) + d;
            }
        }
    }
 
    return $M(a1);
 
}

Vector.prototype.normalize = function() {
 
    var row_length = this.elements.length;
    var t = 0;
 
    for (var i = 0; i < row_length; i++) {
        t += this.elements[i];
    }
 
    return this.multiply((1.0/t));
}


Matrix.prototype.eigenvector = function() {
 
    var tolerance = 0.000001;
 
    var row_length = this.elements[0].length;
 
    var a = [];
 
    for (var i = 0; i < row_length; i++) {
        a.push(1);
    }
 
    var x = $V(a);
 
    var c_old = 0;
 
    for (var i = 0; i < 100; i++) {
        var x_new = x.normalize()
        var c_new = x_new.elements[0];
 
        var e = 100 * (c_new - c_old)/c_new;
        if (Math.abs(e) < tolerance) {
            break;
        }
 
        x = this.multiply(x_new);
        c_old = c_new;
    }
 
    return $V(x);
 
}

Matrix.prototype.pagerank = function() {
    var damping_value = Pages.dampingFactor;
    var row_stochastic_matrix = this.row_stochastic(damping_value);
    var transposed_matrix = row_stochastic_matrix.transpose();
    var eigenvector = transposed_matrix.eigenvector();
    var normalized_eigenvector = eigenvector.normalize();
    document.getElementById("chart5").innerHTML = normalized_eigenvector.elements;
    return normalized_eigenvector.elements;
}