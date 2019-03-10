include <lib/boxmaker.scad>;

// Box inner dimensions
x = 93;
y = 73;
z = 22;

// Material thickness (mm)
thickness = 3.7;

// Tab width (X, Y, Z) (mm)
tabs = [5, 5, 4];

// Uncomment layout_2d line for laser cutting or layout_3d line for 3D object.
layout_2d([x, y, z], thickness) {
//layout_3d([x, y, z], thickness) {

    empty(); // bottom

    difference() {
        side_a([x, y, z], thickness, tabs); // top
        translate([10, y+thickness-12-12.5, 0]) square([39, 12.5]);
        translate([x+thickness-27, y+thickness-18, 0]) circle(16/2);
    }

    difference() {
        side_b([x, y, z], thickness, tabs); // right
        translate([y+thickness-30-18, z+thickness-16, 0]) square([18, 16]);
    }
    
    difference() {
        side_b([x, y, z], thickness, tabs); // left
        translate([-thickness+12, z+thickness-23, 0]) square([13, 23]);
    }

    difference() {
      side_c([x, y, z], thickness, tabs); // front
      translate([x/2, 15, 0]) rotate(180, 0, 0) text("MS", size=8, valign="center", halign="center");
    }

    difference() {    
        side_c([x, y, z], thickness, tabs); // back
        translate([x+thickness-14-23, z+thickness-18, 0]) square([18, 18]);
    }
}
