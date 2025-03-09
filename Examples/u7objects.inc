// This is an object database for Ultima 7 with the Black Gate expansion.

// It names *objects* from the game that are plausibly a single thing with
// nontrivial three-dimensional structure and possible animation. Shapes and
// frames in the game data are treated as *views* to the objects, with
// possibly different facing directions.

// The default facing direction is south. Rule of thumb is that the "face" of
// an object is the direction from which a human interacts with it. So the
// actual face for living things and the side where the human operator is on
// for machines. If it's neither a creature or an operated machine, go with
// a "face" that is symmetrical and has more geometric complexity visible. The
// game's set of shapes has objects facing all cardinal directions.

// This description is written in a way that can be used as a C++ include
// fragment if suitable state machine operating functions are defined outside
// it that match the definition commands. You can probably also write a parser
// for it.

// Syntax:

// The naming function picks the category of the object, all use the same
// parameter convention.

// obj([name]);  // Start defining a new named object.
//
// ## Category definitions
//
// - prop           // Catch-all category for mostly built large objects that need a custom mesh.
// - human          // Human character that can use human animations
// - creature       // Nonhuman creature that is likely to need custom mesh and animation
// - item           // Small item that characters can pick up and carry.
// - tree           // Trees
// - rock           // Natural rocks
// - plant          // Miscellanous plants.
// - pile           // Item that forms piles, like coins and lockpicks
// - ammo           // Item that forms piles and has projectile frames.
// - weapon         // Item has attack frames
// - shield         // Default orientation is flat, can have upright frames.
//
// Items are assumed to be lying flat on the ground. Shields have both an item
// frame (flat) and upright frames.
//
// Definition variants
// - prop("objname", 321);
//   All frames in shape 321 make up object's south-facing view.
// - prop("objname", 321, 0);
//   The first frame in shape 321 is the object's south-facing view.
// - prop("objname", 321, 5, 7);
//   Views are the sequence of frames from 5 to 7 (inclusive).
// - prop("objname")
//   Object has no south-facing view, subsequent command will declare an
//   alternative view.
//
// The other variants mostly work similarly. Character definitions are
// different in that character shapes always have 16 north-facing frames and
// then 16 south-facing frames, so a character will always be defined like
//
// - human(321)
//
// Which will then declare the first 16 frames of shape 321 as north views and
// the rest as south views.
//
// ## Alternative views
//
// If a view shows the object from a different direction, it can be declared
// with the name of the direction.
//
// - east(321);        // All frames from shape 321 show the object facing east
// - east(321, 0);     // Only first frame from shape 321
// - east(321, 5, 7);  // Frames 5 to 7 (inclusive) from shape 321
//
// Similarly for north, west and south. South is the default view so it should
// not be usually needed, as the frames are already specified when the object
// is being declared. Sometimes there might be identical frames in later
// shapes that should be included in the south-facing view though.
//
// There's also
// - alt(shape, [frame, [frame2]])
// for declaring general extended or duplicate view for the default
// south-facing one. It's used with character definitions that don't really
// use defined facings.
//
// ## Offset adjustment
//
// - offset(x, y);
//   The previous view's sprite needs to be shifted by +x, +y pixels. This is
//   used to center sprites on their bounding box base.
//
// ## Large objects
//
// Object offsets are given in 3D tile coordinates. Tile coordinates are
// converted to pixels in the following way:
//   x = tile_x * 8 - tile_z * 4
//   y = tile_y * 8 - tile_z * 4
//
// - extend(tile_x, tile_y, tile_z);
//   Next declared view is added as an extension to the main object offset by
//   the given tile coordinates. Use this when the view isn't south-facing and
//   must be specified separately.
// - extend(tile_x, tile_y, tile_z, shape);
//   Extend the object with all frames from the given shape.
// - extend(tile_x, tile_y, tile_z, shape, frame);
//   Extend the object with the given frame from the given shape.
// - extend(tile_x, tile_y, tile_z, shape, f1, f2);
//   Extend the object with frames from f1 to f2 (inclusive) from shape.

human("mage-male", 154); alt(445);

human("ferryman", 155);

prop("axle-stand", 158, 0); east(158, 1);

prop("stand-cylinder", 158, 2);

item("pocketwatch", 159);

plant("greer-plant", 160);

rock("rock-outcropping-0", 163, 0);
rock("rock-outcropping-1", 163, 1);
rock("rock-outcropping-2", 163, 2);

prop("alchemist-device", 177);

item("cloth-map", 178);

tree("tree-0", 181, 0);
tree("tree-1", 181, 1);
tree("tree-2", 181, 2);
tree("tree-3", 181, 3);

rock("caddelite-meteor", 184, 3);
extend(-4, -4, 0, 184, 0);
extend(0, -4, 0, 184, 1);
extend(-4, 0, 0, 184, 2);

tree("dead-tree-0", 185, 0);
tree("dead-tree-1", 185, 1);
tree("dead-tree-2", 185, 2);
tree("dead-tree-3", 185, 3);
tree("dead-tree-4", 185, 4);
tree("dead-tree-5", 185, 5);
tree("dead-tree-6", 185, 6);

// Fortress is a cube so no real point having it here.
// I just like it as the definitive full-block-cube obj.
prop("fortress", 191);

prop("knights-bridge-piece-0", 194, 0);
prop("knights-bridge-piece-1", 194, 1);
prop("knights-bridge-piece-2", 194, 2);

prop("mountain-corner-0", 195, 4); north(195, 0); east(195, 2); west(195, 6);
prop("mountain-corner-1", 195, 5); north(195, 1); east(195, 3); west(195, 6);
prop("mountain-corner-2", 195, 9); north(195, 8); east(195, 10); west(195, 11);

prop("mast", 199);

prop("trap", 200, 0);

prop("debris-0", 201, 0);
prop("debris-1", 201, 1);
prop("debris-2", 201, 2);
prop("debris-3", 201, 3);
prop("debris-4", 201, 4);
prop("debris-5", 201, 5);
prop("debris-6", 201, 6);
prop("debris-7", 201, 7);
prop("debris-8", 201, 8);
prop("debris-9", 201, 9);
prop("debris-10", 201, 10);
prop("debris-11", 201, 11);
prop("debris-12", 201, 12);

prop("large-debris-0", 202, 0);
prop("large-debris-1", 202, 1);
prop("large-debris-2", 202, 2);
prop("large-debris-3", 202, 3);

prop("chicken-coop", 210, 0); east(210, 1); north(210, 2); west(210, 3);

// Identical to sage-male.
human("alagner", 227);

human("papa", 228);

human("mama", 229);

creature("ethereal-monster", 230);

prop("platform-0", 233);

prop("sphere-generator", 237);
extend(-8, -8, 0, 234);
extend(0, -8, 0, 235);
extend(-8, 0, 0, 236);

prop("cube-generator", 241);
extend(-8, -8, 0, 238);
extend(0, -8, 0, 239);
extend(-8, 0, 0, 240);

prop("tetrahedron-generator", 245);
extend(-8, -8, 0, 242);
extend(0, -8, 0, 243);
extend(-8, 0, 0, 244);

human("paladin", 247); alt(464);

prop("grandfather-clock", 695); east(252);

prop("keg", 258);

human("fighter-male", 259); alt(462);

prop("loom", 261);

human("townsman", 265);

prop("bloodfang-superfighter", 267, 1); extend(0, 0, 3, 267, 0);

prop("mirror", 268);

human("gargoyle-warrior", 274);

prop("crossbeam", 277); east(276);

weapon("musket", 278);

prop("desk", 407, 0); north(407, 1); east(283, 0); west(283, 1);

prop("sundial", 284);

item("cloak-0", 285, 0);
item("cloak-1", 285, 1);
item("cloak-2", 285, 2);

prop("wooden-seat", 292, 2); north(292, 0); east(292, 1); west(292, 3);
prop("red-seat", 292, 6); north(292, 4); east(292, 5); west(292, 7);
prop("blue-seat", 292, 14); north(292, 12); east(292, 13); west(292, 15);

prop("ankh-throne", 292, 18); extend(1, 0, 0, 897, 0);
prop("skull-throne", 292, 22); extend(1, 0, 0, 897, 1);

prop("gray-seat", 292, 30); north(292, 28); east(292, 29); west(292, 31);
// TODO: Figure out the partial bench parts.

item("wedding-ring", 295);
item("ring-of-invisibility", 296);
item("ring-of-protection", 297);
item("ring-of-regeneration", 298);

human("ghost-female", 299);

prop("cooking-utensils-0", 300, 0, 1);
prop("cooking-utensils-1", 300, 2, 3);
prop("cooking-utensils-2", 300, 4, 5);
prop("cooking-utensils-3", 300, 6, 7);
prop("cooking-utensils-4", 300, 8, 9);

// TODO 301 "cart", I don't understand what this is supposed to be...

prop("pumpkin-small", 302, 1); south(302, 3);
prop("pumpkin-medium", 302, 0); south(302, 4); south(302, 5);
prop("pumpkin-large", 302, 2);
prop("pumpkin-rotten", 302, 7, 9);
prop("pumpkin-rotten-large", 302, 10);

human("blacksmith", 304);

prop("black-gate", 305);

tree("evergreen-0", 306, 0);
tree("evergreen-1", 306, 1);
tree("evergreen-2", 306, 2);
tree("evergreen-3", 306, 3);
tree("evergreen-4", 306, 4);
tree("evergreen-5", 306, 5);
tree("evergreen-6", 306, 6);
tree("evergreen-7", 306, 7);
tree("evergreen-8", 306, 8);

prop("laboratory-burner", 307);

tree("fallen-tree-0", 309, 0);
tree("fallen-tree-1", 309, 1);
tree("fallen-tree-2", 309, 2);

tree("tree-4", 310, 0);
tree("tree-5", 310, 1);
tree("tree-6", 310, 2);
tree("tree-7", 310, 3);
tree("tree-8", 310, 4);

prop("trophy-0", 311, 0); east(409, 0);
prop("trophy-1", 311, 1); east(409, 1);
prop("trophy-2", 311, 2); east(409, 2);
prop("trophy-3", 311, 3); east(409, 3);
prop("trophy-4", 311, 4); east(409, 4);
prop("trophy-5", 311, 5); east(409, 5);
prop("trophy-6", 311, 6); east(409, 6);
prop("trophy-7", 311, 7); east(409, 7);
// Tricky, the troll and gargoyle switch places on east-facing shape.
prop("trophy-8", 311, 8); east(409, 9);

prop("trophy-9", 311, 9); east(409, 8);
prop("trophy-10", 311, 10); east(409, 10);
prop("trophy-11", 311, 11); east(409, 11);
prop("trophy-12", 311, 12); east(409, 12);
prop("trophy-13", 311, 13);
// Also this last one has different hair.
prop("trophy-14"); east(409, 13);

prop("gargoyle-futon", 312); east(363);

tree("stump-0", 313, 0);
tree("stump-1", 313, 1);
tree("stump-2", 313, 2);
tree("stump-3", 313, 3);
tree("stump-4", 313, 4);

plant("weed-0", 314, 0);
plant("weed-1", 314, 1);
plant("weed-2", 314, 2);
plant("weed-3", 314, 3);
plant("weed-4", 314, 4);
plant("weed-5", 314, 5);
plant("weed-6", 314, 6);
plant("weed-7", 314, 7);
plant("weed-8", 314, 8);
plant("weed-9", 314, 9);
plant("weed-10", 314, 10);
plant("weed-11", 314, 11);
plant("weed-12", 314, 12);
plant("weed-13", 314, 13);
plant("weed-14", 314, 14);
plant("weed-15", 314, 15);
plant("weed-16", 314, 16);
plant("weed-17", 314, 17);

tree("fallen-tree-3", 315, 0);
tree("fallen-tree-4", 315, 1);
tree("fallen-tree-5", 315, 2);
tree("fallen-tree-6", 315, 3);
tree("fallen-tree-7", 315, 4);

rock("large-rock-0", 316, 0);
rock("large-rock-1", 316, 1);
rock("large-rock-2", 316, 2);
rock("large-rock-3", 316, 3);

human("ghost-male", 317);

human("sage-male", 318);

human("peasant-male", 319);

plant("brambles-0", 320, 0);
plant("brambles-1", 320, 1);
plant("brambles-2", 320, 2);
plant("brambles-3", 320, 3);
plant("brambles-4", 320, 4);
plant("brambles-5", 320, 5);

plant("reeds-0", 321, 0);
plant("reeds-1", 321, 1);
plant("reeds-2", 321, 2);
plant("reeds-3", 321, 3);
plant("reeds-4", 321, 4);
plant("reeds-5", 321, 5);
plant("reeds-6", 321, 6);
plant("reeds-7", 321, 7);
plant("reeds-8", 321, 8);
plant("reeds-9", 321, 9);
plant("reeds-10", 321, 10);
plant("reeds-11", 321, 11);

plant("cattails-0", 323, 0);
plant("cattails-1", 323, 1);
plant("cattails-2", 323, 2);
plant("cattails-3", 323, 3);

tree("dead-tree-7", 325, 0);
tree("dead-tree-8", 325, 1);
tree("dead-tree-9", 325, 2);
tree("dead-tree-10", 325, 3);

plant("tropical-plant-0", 326, 0);
plant("tropical-plant-1", 326, 1);
plant("tropical-plant-2", 326, 2);

tree("cypress-tree-0", 327, 0);
tree("cypress-tree-1", 327, 1);
tree("cypress-tree-2", 327, 2);
tree("cypress-tree-3", 327, 3);
tree("cypress-tree-4", 327, 4);
tree("cypress-tree-5", 327, 5);

tree("baobab-tree-0", 328, 0);
tree("baobab-tree-1", 328, 1);
tree("baobab-tree-2", 328, 2);
tree("baobab-tree-3", 328, 3);

item("virtue-stone-0", 330, 0);
item("virtue-stone-1", 330, 1);
item("virtue-stone-2", 330, 2);
item("virtue-stone-3", 330, 3);
item("virtue-stone-4", 330, 4);
item("virtue-stone-5", 330, 5);
item("virtue-stone-6", 330, 6);
item("virtue-stone-7", 330, 7);

rock("rock-0", 331, 0);
rock("rock-1", 331, 1);
rock("rock-2", 331, 2);
rock("rock-3", 331, 3);
rock("rock-4", 331, 4);
rock("rock-5", 331, 5);
rock("rock-6", 331, 6);
rock("rock-7", 331, 7);
rock("rock-8", 331, 8);
rock("rock-9", 331, 9);
rock("rock-10", 331, 10);
rock("rock-11", 331, 11);
rock("rock-12", 331, 12);

tree("tree-9", 332, 0);
tree("tree-10", 332, 1);

prop("table-0", 333);

item("light-source-0", 336, 0);
item("light-source-1", 336, 1);
item("light-source-2", 336, 2);
item("light-source-3", 336, 3);
item("light-source-4", 336, 4); east(336, 5);
item("light-source-5", 336, 6);
item("light-source-6", 336, 7);
item("light-source-7", 336, 8);
item("light-source-8", 336, 9);
item("light-source-9", 336, 10);
item("light-source-10", 336, 11);
item("light-source-11", 336, 12);
item("light-source-12", 336, 13);
item("light-source-13", 336, 14);
item("light-source-14", 336, 15);
item("light-source-15", 336, 16);

creature("ghost", 337);

item("lit-light-source-0", 338, 0);
item("lit-light-source-1", 338, 1);
item("lit-light-source-2", 338, 2);
item("lit-light-source-3", 338, 3);
item("lit-light-source-4", 338, 4); east(338, 5);
item("lit-light-source-5", 338, 6);
item("lit-light-source-6", 338, 7);
item("lit-light-source-7", 338, 8);
item("lit-light-source-8", 338, 9);
item("lit-light-source-9", 338, 10);
item("lit-light-source-10", 338, 11);
item("lit-light-source-11", 338, 12);
item("lit-light-source-12", 338, 13);
item("lit-light-source-13", 338, 14);
item("lit-light-source-14", 338, 15);
item("lit-light-source-15", 338, 16);

item("blue-potion", 340, 0);
item("yellow-potion", 339, 1);
item("red-potion", 338, 2);
item("green-potion", 337, 3);
item("orange-potion", 336, 4);
item("purple-potion", 335, 5);
item("white-potion", 334, 6);
item("black-potion", 333, 7);
item("red-yellow-potion", 332, 8);

rock("rock-13", 341, 0);
rock("rock-14", 341, 1);
rock("rock-15", 341, 2);
rock("rock-16", 341, 3);
rock("rock-17", 341, 4);
rock("rock-18", 341, 5); south(341, 6);
rock("rock-19", 341, 7);
rock("rock-20", 341, 8);
rock("rock-21", 341, 9);
rock("rock-22", 341, 10); east(341, 11);

rock("boulder-0", 342, 0);
rock("boulder-1", 342, 1);
rock("boulder-2", 342, 2);
rock("boulder-3", 342, 3);

rock("boulder-4", 343, 0);
rock("boulder-5", 343, 1);
rock("boulder-6", 343, 2);

item("greaves", 353);

human("red-liche", 354);

prop("sign-0", 360, 0); east(361, 0);
prop("sign-1", 360, 1); east(361, 1);
prop("sign-2", 360, 2); east(361, 2);
prop("sign-3", 360, 3); east(361, 3);
prop("sign-4", 360, 4); east(361, 4);
prop("sign-5", 360, 5); east(361, 5);
prop("sign-6", 360, 6); east(361, 6);
prop("sign-7", 360, 7); east(361, 7);
prop("sign-8", 360, 8); east(361, 8);
prop("sign-9", 360, 9); east(361, 9);
prop("sign-10", 360, 10); east(361, 10);
prop("sign-11", 360, 11); east(361, 11);
prop("sign-12", 360, 12); east(361, 12);

prop("platform-1", 364);

creature("unicorn", 375);

item("food-item-0", 377, 0);
item("food-item-1", 377, 1);
item("food-item-2", 377, 2);
item("food-item-3", 377, 3);
item("food-item-4", 377, 4);
item("food-item-5", 377, 5);
item("food-item-6", 377, 6);
item("food-item-7", 377, 7);
item("food-item-8", 377, 8);
item("food-item-9", 377, 9);
item("food-item-10", 377, 10);
item("food-item-11", 377, 11);
item("food-item-12", 377, 12);
item("food-item-13", 377, 13);
item("food-item-14", 377, 14);
item("food-item-15", 377, 15);
item("food-item-16", 377, 16);
item("food-item-17", 377, 17);
item("food-item-18", 377, 18);
item("food-item-19", 377, 19);
item("food-item-20", 377, 20);
item("food-item-21", 377, 21);
item("food-item-22", 377, 22);
item("food-item-23", 377, 23);
item("food-item-24", 377, 24);
item("food-item-25", 377, 25);
item("food-item-26", 377, 26);
item("food-item-27", 377, 27);
item("food-item-28", 377, 28);
item("food-item-29", 377, 29);
item("food-item-30", 377, 30);
item("food-item-31", 377, 31);

prop("sign-13", 379, 7); east(379, 0);
prop("sign-14", 379, 9); east(379, 1);
prop("sign-15", 379, 8); east(379, 2);
prop("sign-16", 379, 10); east(379, 3);
prop("sign-17"); east(379, 4);
// XXX: I don't really understand what frames 5 and 6 are supposed to be.

creature("cyclops", 380); alt(501);

creature("three-headed-hydra", 381);

creature("kissme", 382);

item("magic-helm", 383);

// Rising step heights.
prop("wooden-stair-0", 426, 0); east(385, 0);
prop("wooden-stair-1", 427, 0); east(385, 0);
prop("wooden-stair-2", 428, 0); east(385, 0);
prop("wooden-stair-3", 429, 0); east(973, 0);
prop("wooden-stair-4", 430, 0); east(974, 0);

prop("stone-stair-0", 426, 1); east(385, 1);
prop("stone-stair-1", 427, 1); east(385, 1);
prop("stone-stair-2", 428, 1); east(385, 1);
prop("stone-stair-3", 429, 1); east(973, 1);
prop("stone-stair-4", 430, 1); east(974, 1);

item("eating-utensil-0", 388, 0); north(388, 8);
item("eating-utensil-1", 388, 1); north(388, 9);
item("eating-utensil-2", 388, 2); north(388, 10);
item("eating-utensil-3", 388, 3); north(388, 11);
item("eating-utensil-4", 388, 4); north(388, 12);
item("eating-utensil-5", 388, 5); north(388, 13);

prop("stalagmite-0", 389, 0);
prop("stalagmite-1", 389, 1);
prop("stalagmite-2", 389, 4);
prop("stalagmite-3", 389, 5);
prop("stalagmite-4", 389, 6);
prop("stalagmite-5", 389, 7);

prop("stalactite-0", 389, 2);
prop("stalactite-1", 389, 3);

prop("stalagmite-6", 391, 0);
prop("stalagmite-7", 391, 1);
prop("stalagmite-8", 391, 2);
prop("stalagmite-9", 391, 3);
prop("stalagmite-10", 391, 4);
prop("stalagmite-11", 391, 5);

human("blue-guard", 394);

prop("mountain-slope-0", 395, 4); north(395, 0); east(395, 2); west(395, 6);
prop("mountain-slope-1", 395, 5); north(395, 1); east(395, 3); west(395, 7);

prop("mountain_inverse_corner_0", 396, 0); east(396, 2); west(396, 4); north(396, 6);
prop("mountain_inverse_corner_1", 396, 1); east(396, 3); west(396, 5); north(396, 7);

// XXX: These could be named to which character type's body they are..
prop("body-avatar-male", 400, 1);
prop("body-avatar-female", 400, 2);
prop("body-2", 400, 3);
prop("body-3", 400, 4);
prop("body-4", 400, 5);
prop("body-5", 400, 6);
prop("body-6", 400, 7);
prop("body-7", 400, 8);
prop("body-8", 400, 9);
prop("body-9", 400, 10);
prop("body-10", 400, 11);
prop("body-11", 400, 12);
prop("body-12", 400, 13);
prop("body-13", 400, 14);
prop("body-14", 400, 15);
prop("body-15", 400, 16);
prop("body-16", 400, 17);
prop("body-17", 400, 18);
prop("body-18", 400, 19);
prop("body-19", 400, 20);
prop("body-20", 400, 21);
prop("body-21", 400, 22);
prop("body-22", 400, 23);
prop("body-23", 400, 24);
prop("body-24", 400, 25);
prop("body-25", 400, 26);
prop("body-26", 400, 27);
prop("body-27", 400, 28);
prop("body-28", 400, 29);
prop("body-29", 400, 30);
prop("body-30", 400, 31);

human("pirate", 401); alt(458);

human("batlin", 403); alt(482);

prop("nightstand", 406, 1); east(406, 0);

prop("mining-machine");
east(411);
extend(-7, 2, 0); east(410);

prop("black-gate-dais", 412, 4);
extend(-5, -5, 0, 412, 0);
extend( 0, -5, 0, 412, 1);
extend( 5, -5, 0, 412, 2);
extend(-5,  0, 0, 412, 3);
extend( 5,  0, 0, 412, 5);
extend(-5,  5, 0, 412, 6);
extend( 0,  5, 0, 412, 7);
extend( 5,  5, 0, 412, 8);

prop("body-31", 414, 1);
prop("body-32", 414, 2);
prop("body-33", 414, 3);
prop("body-34", 414, 4);
prop("body-35", 414, 5);
prop("body-36", 414, 6);
prop("body-kissme", 414, 7); // Took a moment to figure out what this is...
prop("body-37", 414, 9);
prop("body-38", 414, 10);
prop("body-39", 414, 11);
// Decapitated British gets a fun spreading blood pool animation.
prop("body-decapitated-british", 414, 30); south(414, 31); south(414, 12);
prop("body-plaque-british", 414, 13);
prop("body-42", 414, 14);
prop("body-43", 414, 15);
prop("body-44", 414, 16);
prop("body-45", 414, 17);
prop("body-iolo", 414, 18);
prop("body-shamino", 414, 19);
prop("body-dupre", 414, 20);
prop("body-spark", 414, 21);
prop("body-jaana", 414, 22);
prop("body-51", 414, 23);
prop("burned-body", 414, 24); north(414, 25);
prop("body-inamo", 414, 26);
prop("body-mama", 414, 27);
prop("body-papa", 414, 28);

prop("garbage-0", 415, 0);
prop("garbage-1", 415, 1);
prop("garbage-2", 415, 2);
prop("garbage-3", 415, 3);
prop("garbage-4", 415, 4);
prop("garbage-5", 415, 5);
prop("garbage-6", 415, 6);
prop("garbage-7", 415, 7);
prop("garbage-8", 415, 8);
prop("garbage-9", 415, 9);
prop("garbage-10", 415, 10);
prop("garbage-11", 415, 11);
prop("garbage-12", 415, 12);
prop("garbage-13", 415, 13);
prop("garbage-14", 415, 14);
prop("garbage-15", 415, 15);
prop("garbage-16", 415, 16);
prop("garbage-17", 415, 17);
prop("garbage-18", 415, 18);
prop("garbage-19", 415, 19);
prop("garbage-20", 415, 20);
prop("garbage-21", 415, 21);
prop("garbage-22", 415, 22);
prop("garbage-23", 415, 23);
prop("garbage-24", 415, 24);
prop("garbage-25", 415, 25);
prop("garbage-26", 415, 26);
prop("garbage-27", 415, 27);
prop("garbage-28", 415, 28);
prop("garbage-29", 415, 29);
prop("garbage-30", 415, 30);
prop("garbage-31", 415, 31);

prop("drawers", 416); east(679);

ammo("magic-bolt", 417);

prop("giant-skull-0", 418, 0);
prop("giant-skull-1", 418, 1);
prop("giant-skull-2", 418, 2);

prop("giant-spine", 418, 3);
extend(0, -18, 0, 418, 6);
extend(0, -12, 0, 418, 5);
extend(0, -6, 0, 417, 4);

plant("crops-0", 423, 0);
plant("crops-1", 423, 1);
plant("crops-2", 423, 2);
plant("crops-3", 423, 3);
plant("crops-4", 423, 4);
plant("crops-5", 423, 5);
plant("crops-6", 423, 6);
plant("crops-7", 423, 7);
plant("crops-8", 423, 8);
plant("crops-9", 423, 9);
plant("crops-10", 423, 10);
plant("crops-11", 423, 11);
plant("crops-12", 423, 12);
plant("crops-13", 423, 13);
plant("crops-14", 423, 14);
plant("crops-15", 423, 15);
plant("crops-16", 423, 16);
plant("crops-17", 423, 17);
plant("crops-18", 423, 18);
plant("crops-19", 423, 19);
plant("crops-20", 423, 20);
plant("crops-21", 423, 21);
plant("crops-22", 423, 22);
plant("crops-23", 423, 23);
plant("crops-24", 423, 24);
plant("crops-25", 423, 25);
plant("crops-26", 423, 26);
plant("crops-27", 423, 27);

prop("red-carpet-stairs-left-0", 426, 2);
prop("red-carpet-stairs-left-1", 427, 2);
prop("red-carpet-stairs-left-2", 428, 2);
prop("red-carpet-stairs-left-3", 429, 2);
prop("red-carpet-stairs-left-4", 430, 2);

prop("red-carpet-stairs-right-0", 426, 3);
prop("red-carpet-stairs-right-1", 427, 3);
prop("red-carpet-stairs-right-2", 428, 3);
prop("red-carpet-stairs-right-3", 429, 3);
prop("red-carpet-stairs-right-4", 430, 3);

prop("black-carpet-stairs-left-0", 426, 4);
prop("black-carpet-stairs-left-1", 427, 4);
prop("black-carpet-stairs-left-2", 428, 4);
prop("black-carpet-stairs-left-3", 429, 4);
prop("black-carpet-stairs-left-4", 430, 4);

prop("black-carpet-stairs-right-0", 426, 5);
prop("black-carpet-stairs-right-1", 427, 5);
prop("black-carpet-stairs-right-2", 428, 5);
prop("black-carpet-stairs-right-3", 429, 5);
prop("black-carpet-stairs-right-4", 430, 5);

prop("bellows-0", 431, 0, 2);
prop("bellows-1", 431, 3, 5);

prop("cask"); east(434);

prop("lit-sconce", 435);

prop("wagon-floor", 436);

prop("wagon-wheel", 437);

item("hood-0", 444);
item("hood-1", 445);

human("mage-female", 446);

human("wounded-man", 447);

human("sage-female", 448);

// These guys might not use the same animations as other humans, so should
// they be classified as creatures?
human("beggar-crutches", 449);
human("beggar-legless", 450);

human("nobleman", 451);

human("peasant-female", 452);

tree("tree-11", 453, 0);
tree("tree-12", 453, 1);
tree("tree-13", 453, 2);
tree("tree-14", 453, 3);
tree("tree-15", 453, 4);
tree("tree-16", 453, 5);
tree("tree-17", 453, 6);
tree("tree-18", 453, 7);
tree("tree-19", 453, 8);

human("shopkeeper-female", 454);
human("shopkeeper-male", 455);

human("noblewoman", 456);

human("gypsy-male", 457);

human("wench", 459);

human("ranger-male", 460);
human("ranger-female", 461);

human("fighter-female", 463);

human("iolo", 465);

human("lord-british", 466);

human("jester", 467);

human("entertainer-male", 468);
human("entertainer-female", 469);

prop("well-0", 470, 0);
prop("well-1", 470, 1);

human("peasant-child", 471); alt(965);

human("noble-child", 472);

human("gargoyle-noble", 473);

weapon("sling", 474);

human("gargoyle-worker", 475);

creature("smith-the-horse", 476);

creature("frank-the-fox", 477);

creature("sherry-the-mouse", 478);

creature("emp", 479); alt(784);

human("draxinusom", 480);

prop("sconce", 481);

prop("bars-0", 484, 0);
prop("bars-1", 484, 1);
prop("bars-2", 484, 2);
prop("bars-3", 484, 3);

human("highwayman", 485);

prop("big-statue-0", 486, 0);
prop("big-statue-1", 486, 1);
prop("big-statue-2", 486, 2);
prop("big-statue-3"); east(486, 3); west(486, 4);
prop("big-statue-4", 486, 5);

human("shamino", 487);

human("dupre", 488);

human("spark", 489);

human("jaana", 490);

creature("acid-slug", 491);

creature("alligator", 492);

creature("bat", 493);

creature("bee", 494);

creature("cat", 495);

creature("dog", 496);

creature("chicken", 498);

creature("corpser", 499);

creature("cow", 500);

creature("deer", 502);

prop("puppet-show", 503);

creature("dragon", 504);

creature("drake", 505);

human("hook", 506);

prop("corpse-0", 507, 0);
prop("corpse-1", 507, 1);
prop("corpse-2", 507, 2);
prop("corpse-3", 507, 3);
prop("corpse-4", 507, 4);
prop("corpse-5", 507, 5);
prop("corpse-6", 507, 6);
prop("corpse-7", 507, 7);
prop("corpse-8", 507, 8);
prop("corpse-9", 507, 9);

prop("fish", 509);

creature("fox", 510);

creature("gazer", 511);

creature("gremlin", 513);

creature("headless", 514);

prop("drawbridge", 515); south(870);

human("black-liche", 519);

prop("virtue-roulette-table", 520);

creature("mouse", 521);

prop("locked-chest", 522, 0); east(522, 1);

creature("rat", 523);

creature("reaper", 524);

creature("sea-serpent", 525);

prop("lit-lamp-0", 526, 0); east(525, 1);
prop("lit-lamp-1", 526, 2);
prop("lit-lamp-2", 526, 3);
prop("lit-lamp-3", 526, 4);
prop("lit-lamp-4", 526, 5);
prop("lit-lamp-5", 526, 6);
prop("lit-lamp-6", 526, 7);
prop("lit-lamp-7", 526, 8);
prop("lit-lamp-8", 526, 9);
prop("lit-lamp-9", 526, 10);
prop("lit-lamp-10", 526, 11);
prop("lit-lamp-11", 526, 12);
prop("lit-lamp-12", 526, 13);

human("skeleton", 528);

// 529 is slime, it's weird and not a coherent single object.

creature("snake", 530);

// Game uses "harpie", that isn't right.
creature("harpy", 532);

creature("troll", 533); alt(861);

creature("wisp", 534);

prop("spent-sconce", 535);

creature("tentacles", 536);

creature("wolf", 537);

item("chain-coif", 539);

item("great-helm", 541);

item("crested-helm", 542);

shield("buckler", 543, 0); east(543, 1); south(543, 2);

prop("broken-column-0", 544, 0);
prop("broken-column-1", 544, 1);
prop("broken-column-2", 544, 2);
prop("broken-column-3", 544, 3);
prop("broken-column-4", 544, 4);
prop("broken-column-5", 544, 5);
prop("broken-column-6", 544, 6);
prop("broken-column-7", 544, 7);
prop("broken-column-8", 544, 8);
prop("broken-column-9", 544, 9);

shield("curved-heater", 545, 0); east(545, 1); south(545, 2);

item("broken-dish-0", 546, 0);
item("broken-dish-1", 546, 1);
item("broken-dish-2", 546, 2);
item("broken-dish-3", 546, 3);
item("broken-dish-4", 546, 4);
item("broken-dish-5", 546, 5);
item("broken-dish-6", 546, 6);
item("broken-dish-7", 546, 7);
item("broken-dish-8", 546, 8);
item("broken-dish-9", 546, 9);
item("broken-dish-10", 546, 10);
item("broken-dish-11", 546, 11);
item("broken-dish-12", 546, 12);
item("broken-dish-13", 546, 13);
item("broken-dish-14", 546, 14);
item("broken-dish-15", 546, 15);

weapon("magic sword", 547);

weapon("hoe-of-destruction", 548);

weapon("lightning-whip", 549);

weapon("magic-boomerang", 550);

weapon("fire-sword", 551);

weapon("magic-axe", 552);

weapon("firedoom-staff", 553);

ammo("burst-arrow", 554);

weapon("hawk", 555);

ammo("magic-arrow", 556);

weapon("juggernaut-hammer", 557);

ammo("lucky-arrow", 558);

weapon("magebane", 559);

ammo("love-arrow", 560);

weapon("great-dagger", 561);

weapon("death-scythe", 562);

weapon("blowgun", 563);

weapon("poison-dagger", 564);

weapon("sword-of-defense", 567);

ammo("tseramed-arrow", 568);

item("leather-armour", 569);

item("scale-armour", 570);

item("chain-armour", 571);

shield("wooden-shield", 572, 0); east(572, 1); south(572, 2);

item("plate-armour", 573);

item("leather-leggings", 574);

item("chain-leggings", 575);

item("plate-leggings", 576);

item("gate-pedestal-0", 577, 0); south(577, 3);
item("gate-pedestal-1", 577, 1); south(577, 4);
item("gate-pedestal-2", 577, 2); south(577, 5);

shield("spiked-shield", 578, 0); east(578, 1); south(578, 2);

item("leather-gloves", 579);

item("gauntlets", 580);

ammo("ammunition", 581);  // For the musket?

item("leather-collar", 582);

item("bed-roll", 583);

item("kidney-belt", 584);

item("shoes", 585);

item("gorget", 586);

item("boots", 587, 0);
item("magic-boots", 587, 1);

item("swamp-boots", 588);

weapon("pitchfork", 589);

weapon("club", 590);

weapon("main-gauche", 591);

weapon("spear", 592);

weapon("throwing-axe", 593);

weapon("dagger", 594);

weapon("torch", 595);

weapon("morning-star", 596);

weapon("bow", 597);

weapon("crossbow", 598);

weapon("sword", 599);

weapon("two-handed-hammer", 600);

weapon("two-handed-axe", 601);

weapon("two-handed-sword", 602);

weapon("halberd", 603);

weapon("glass-sword", 604);

weapon("boomerang", 605);

weapon("magic-bow", 606);

weapon("decorative-sword", 608);

shield("kite-shield", 609, 0); east(609, 1); south(609, 2);

plant("lily-pads-0", 611, 0);
plant("lily-pads-1", 611, 1);

weapon("cleaver", 614);

weapon("knife", 615);

item("bottle-0", 616, 0);
item("bottle-1", 616, 1);
item("bottle-2", 616, 2);
item("bottle-3", 616, 3);
item("bottle-4", 616, 4);
item("bottle-5", 616, 5);
item("bottle-6", 616, 6);
item("bottle-7", 616, 7);
item("bottle-8", 616, 8);
item("bottle-9", 616, 9);
item("bottle-10", 616, 10);
item("bottle-11", 616, 11);
item("bottle-12", 616, 12);
item("bottle-13", 616, 13);
item("bottle-14", 616, 14);
item("bottle-15", 616, 15);

human("time-lord", 617);

weapon("scythe", 618);

plant("fern-0", 619, 0);
plant("fern-1", 619, 1);
plant("fern-2", 619, 2);
plant("fern-3", 619, 3);

weapon("rake", 620);

weapon("whip", 622);

weapon("hammer", 623);

weapon("pick", 624);

weapon("shovel", 625);

weapon("hoe", 626);

pile("lockpick", 627);

item("cup-0", 628, 0);
item("cup-1", 628, 1);
item("cup-2", 628, 2);
item("cup-3", 628, 3);
item("cup-4", 628, 4);
item("cup-5", 628, 5);
item("cup-6", 628, 6);
item("cup-7", 628, 7);
item("cup-8", 628, 8);
item("cup-9", 628, 9);
item("cup-10", 628, 10);
item("cup-11", 628, 11);
item("cup-12", 628, 12);
item("cup-13", 628, 13);
item("cup-14", 628, 14);
item("cup-15", 628, 15);

weapon("lightning-wand", 629);

weapon("fire-wand", 630);

// This doesn't seem to go for any sort of 3D structure, just splits the big
// tree sprite into quarters.
tree("big-tree", 631);
extend(-8, -1, 0, 643, 0);
extend(-1, -8, 0, 643, 1);
extend(-8, -8, 0, 643, 2);

prop("table-1", 633, 0);
prop("table-2", 633, 1);
prop("table-3", 633, 2);
prop("table-4", 633, 3);
prop("table-5", 633, 4);
prop("table-6", 633, 5);

rock("standing-stone-0", 634, 0);
rock("standing-stone-1", 634, 1);
rock("standing-stone-2", 634, 2);

weapon("custom-sword", 635);

weapon("serpentine-dagger", 636);

weapon("serpentine-sword", 637);

item("caddelite-helm", 638);

item("ring-0", 640, 0);
item("ring-1", 640, 1);

item("key-0", 641, 0);
item("key-1", 641, 1);
item("key-2", 641, 2);
item("key-3", 641, 3);
item("key-4", 641, 4);
item("key-5", 641, 5);
item("key-6", 641, 6);
item("key-7", 641, 7);
item("key-8", 641, 8);
item("key-9", 641, 9);
item("key-10", 641, 10);
item("key-11", 641, 11);
item("key-12", 641, 12);
item("key-13", 641, 13);
item("key-14", 641, 14);
item("key-15", 641, 15);
item("key-16", 641, 16);
item("key-17", 641, 17);
item("key-18", 641, 18);
item("key-19", 641, 19);
item("key-20", 641, 20);
item("key-21", 641, 21);
item("key-22", 641, 22);
item("key-23", 641, 23);
item("key-24", 641, 24);
item("key-25", 641, 25);
item("key-26", 641, 26);
item("key-27", 641, 27);
item("key-28", 641, 28);
item("key-29", 641, 29);
item("key-30", 641, 30);
item("key-31", 641, 31);

item("open-book-0", 642, 0);
item("open-book-1", 642, 3);
item("open-book-2", 642, 6);
item("open-book-3", 642, 9);
item("open-book-4", 642, 12);
item("open-book-5", 642, 15);
item("open-book-6", 642, 18);
item("open-book-7", 642, 21);
item("open-book-8", 642, 24);
item("open-book-9", 642, 27);

// Books have the same flat/upright dynamic as shields, so use that marker.
shield("book-0", 642, 1); east(642, 2);
shield("book-1", 642, 4); east(642, 5);
shield("book-2", 642, 7); east(642, 8);
shield("book-3", 642, 10); east(642, 11);
shield("book-4", 642, 13); east(642, 14);
shield("book-5", 642, 16); east(642, 17);
shield("book-6", 642, 19); east(642, 20);
shield("book-7", 642, 22); east(642, 23);
shield("book-8", 642, 25); east(642, 26);
shield("book-9", 642, 28); east(642, 29);

pile("gold-coin", 644);

item("gold-nugget", 645, 0); east(645, 1);

item("gold-bar", 646, 0); east(645, 1);

weapon("triple-crossbow", 647);

item("sleeping-powder-jar", 648, 0);
item("sleeping-powder", 648, 1);

item("venom", 649);

item("iron-sextant", 650, 0);
item("gold-sextant", 650, 1);

prop("spinning-wheel", 651);

prop("bale-of-wool", 653, 0); east(653, 1);
prop("broken-bale-of-wool", 653, 2);

item("spindle-0", 654, 0);
item("spindle-1", 654, 1);
item("spindle-2", 654, 2);
item("spindle-3", 654, 3);
item("spindle-4", 654, 4);
item("spindle-5", 654, 5);
item("spindle-6", 654, 6);
item("spindle-7", 654, 7);
item("spindle-8", 654, 8);
item("spindle-9", 654, 9);

prop("topiary-mermaid", 655, 0);
prop("topiary-dolphin", 655, 1);
prop("topiary-owl", 655, 2);
prop("topiary-swan", 655, 3);

prop("wine-press", 656, 0);
prop("wine-press-cask", 656, 1);

prop("curtain-0", 657, 0); east(678, 0);
prop("curtain-1", 657, 1); east(678, 1);
prop("curtain-2", 657, 2); east(678, 2);
prop("curtain-3", 657, 3); east(678, 3);
prop("curtain-4", 657, 4); east(678, 4);
prop("curtain-5", 657, 5); east(678, 5);
prop("curtain-6", 657, 6); east(678, 6);
prop("curtain-7", 657, 7); east(678, 7);
prop("curtain-8", 657, 8); east(678, 8);
prop("curtain-9", 657, 9); east(678, 9);
prop("curtain-10", 657, 10); east(678, 10);
prop("curtain-11", 657, 11); east(678, 11);

item("dough-0", 658, 0);
item("dough-1", 658, 1);
item("dough-2", 658, 2);

weapon("mace", 659);

creature("mongbat", 661);

weapon("fishing-rod", 662);

shield("magic-shield", 663, 0); east(663, 1); south(663, 2);

prop("stove", 872); extend(0, -1, 0, 986); east(664); extend(-1, 0, 0, 712);

item("magic-armor", 666);

prop("cobwebs-0", 667, 0);
prop("cobwebs-1", 667, 1);

item("sword-blank", 668);

plant("cactus-0", 669, 0);
plant("cactus-1", 669, 1);
plant("cactus-2", 669, 2);
plant("cactus-3", 669, 3);
plant("cactus-4", 669, 4);
plant("cactus-5", 669, 5);

tree("maple-tree-0", 670, 0);
tree("maple-tree-1", 670, 1);
tree("maple-tree-2", 670, 2);
tree("maple-tree-3", 670, 3);
tree("maple-tree-4", 670, 4);
tree("maple-tree-5", 670, 5);

plant("mushrooms-0", 671, 0);
plant("mushrooms-1", 671, 1);
plant("mushrooms-2", 671, 2);
plant("mushrooms-3", 671, 3);
plant("mushrooms-4", 671, 4);
plant("mushrooms-5", 671, 5);
plant("mushrooms-6", 671, 6);
plant("mushrooms-7", 671, 7);
plant("mushrooms-8", 671, 8);
plant("mushrooms-9", 671, 9);
plant("mushrooms-10", 671, 10);
plant("mushrooms-11", 671, 11);
plant("mushrooms-12", 671, 12);
plant("mushrooms-13", 671, 13);
plant("mushrooms-14", 671, 14);
plant("mushrooms-15", 671, 15);
plant("mushrooms-16", 671, 16);
plant("mushrooms-17", 671, 17);

plant("shrubbery-0", 672, 0);
plant("shrubbery-1", 672, 1);
plant("shrubbery-2", 672, 2);
plant("shrubbery-3", 672, 3);
plant("shrubbery-4", 672, 4);
plant("shrubbery-5", 672, 5);
plant("shrubbery-6", 672, 6);

plant("plant-0", 673, 0);
plant("plant-1", 673, 1);
plant("plant-2", 673, 2);
plant("plant-3", 673, 3);
plant("plant-4", 673, 4);
plant("plant-5", 673, 5);
plant("plant-6", 673, 6);
plant("plant-7", 673, 7);
plant("plant-8", 673, 8);

tree("silverleaf-tree-0", 674, 0);
tree("silverleaf-tree-1", 674, 1);
tree("silverleaf-tree-2", 674, 2);
tree("silverleaf-tree-3", 674, 3);
tree("silverleaf-tree-4", 674, 4);

item("desk-item-0", 675, 0);
item("desk-item-1", 675, 1);
item("desk-item-2", 675, 2);
item("desk-item-3", 675, 3);
item("desk-item-4", 675, 4);
item("desk-item-5", 675, 5);
item("desk-item-6", 675, 6);
item("desk-item-7", 675, 7);
item("desk-item-8", 675, 8);
item("desk-item-9", 675, 9);
item("desk-item-10", 675, 10);
item("desk-item-11", 675, 11);
item("desk-item-12", 675, 12);
item("desk-item-13", 675, 13);
item("desk-item-14", 675, 14);
item("desk-item-15", 675, 15);
item("desk-item-16", 675, 16);
item("desk-item-17", 675, 17);
item("desk-item-18", 675, 18);
item("desk-item-19", 675, 19);
item("desk-item-20", 675, 20);

item("sack-of-wheat", 677);

item("jar-0", 681, 0);
item("jar-1", 681, 1);
item("jar-2", 681, 2);
item("jar-3", 681, 3);
item("jar-4", 681, 4);
item("jar-5", 681, 5);
item("jar-6", 681, 6);
item("jar-7", 681, 7);
item("jar-8", 681, 8);
item("jar-9", 681, 9);

item("glass-item-0", 682, 0);
item("glass-item-1", 682, 1);
item("glass-item-2", 682, 2);
item("glass-item-3", 682, 3);
item("glass-item-4", 682, 4);
item("glass-item-5", 682, 5);
item("glass-item-6", 682, 6);
item("glass-item-7", 682, 7);
item("glass-item-8", 682, 8);
item("glass-item-9", 682, 9);
item("glass-item-10", 682, 10);
item("glass-item-11", 682, 11);

tree("tree-20", 683, 0);
tree("tree-21", 683, 1);
tree("tree-22", 683, 2);
tree("tree-23", 683, 3);

prop("trellis-0", 684, 0);
prop("trellis-1", 684, 1);
prop("trellis-2", 684, 2);
prop("trellis-3", 684, 3);

prop("mechanism", 685);

item("magic-leggings", 686);

prop("pillar-0", 687, 0);
prop("pillar-1", 687, 1);
prop("pillar-2", 687, 2);
prop("pillar-3", 687, 3);
prop("pillar-4", 687, 4);

prop("harpsichord", 689, 0); east(689, 1);

prop("xylophone", 690);

item("lyre", 691);

item("lute-0", 692, 0);
item("lute-1", 692, 1);

item("whistle-0", 693, 0);
item("whistle-1", 693, 1);
item("whistle-2", 693, 2);

prop("bed-0"); east(696, 0);
prop("bed-1"); east(696, 1);
prop("bed-2"); east(696, 2);
prop("bed-3"); east(696, 3);
prop("bed-4"); east(696, 5);
prop("bed-5"); east(696, 7);
prop("bed-6"); east(696, 9);
prop("bed-7"); east(696, 11);
prop("bed-8"); east(696, 13);
prop("bed-9"); east(696, 15);

// Numbered after the bed prop it matches.
prop("bedsheet-3"); east(696, 4);
prop("bedsheet-4"); east(696, 6);
prop("bedsheet-5"); east(696, 8);
prop("bedsheet-6"); east(696, 10);
prop("bedsheet-7"); east(696, 12);
prop("bedsheet-8"); east(696, 14);
prop("bedsheet-9"); east(696, 16);

prop("podium", 697, 3); west(697, 0); east(697, 1); north(697, 2);

// Are these multiple items or just animation?
item("shears", 698);

weapon("lit-torch", 701);

prop("cannon", 702, 0); west(702, 1); north(702, 2); east(702, 3);

item("cannon-ball", 703);

item("powder-keg", 704);

prop("rope-ladder", 705); east(714);

creature("scorpion", 706);

weapon("black-sword", 707);

prop("stocks", 708);

// TODO: Figure out of the extension is right.
prop("guillotine", 734); extend(0, -2, 0, 709);

item("log-saw", 710);

prop("mill-stone", 711);

prop("post-0", 713, 0);
prop("post-1", 713, 1);
prop("post-2", 713, 2);
prop("post-3", 713, 3);
prop("post-4", 713, 4);
prop("post-5", 713, 5);
prop("post-6", 713, 6);

prop("tombstone-0", 715, 0);
prop("tombstone-1", 715, 1);
prop("tombstone-2", 715, 2);
prop("tombstone-3", 715, 3);
prop("tombstone-4", 715, 4);

creature("bird", 716);

item("plate-0", 717, 0);
item("plate-1", 717, 1);
item("plate-2", 717, 2); east(717, 3);
item("plate-3", 717, 4);
item("plate-4", 717, 5);

prop("pedestal-0", 718, 0);
prop("pedestal-1", 718, 1);
prop("pedestal-2", 718, 2);
prop("pedestal-3", 718, 3);
prop("pedestal-4", 718, 4);
prop("pedestal-5", 718, 5);

prop("water-trough-0", 741, 0, 3); east(719);

human("red-guard", 720); alt(806); alt(946);

human("avatar-male", 721);

ammo("arrow", 722);

ammo("bolt", 723);

prop("fellowship-icon"); east(724);

prop("virtue-roulette-wheel", 725);

prop("blue-lens", 726, 0);
prop("purple-lens", 726, 1);

creature("horse", 727);

item("caddelite-chunks-0", 728, 0);
item("caddelite-chunks-1", 728, 1);
item("caddelite-chunks-2", 728, 2);
item("caddelite-chunks-3", 728, 3);
item("caddelite-chunks-4", 728, 4);

item("crystal-ball", 729);

item("baby-0", 730, 1); east(730, 0);
item("baby-1"); east(730, 2);

prop("archery-target"); east(735);

item("pants-0", 738, 0);
item("pants-1", 738, 1);
item("pants-2", 738, 2);
item("pants-3", 738, 3);
item("pants-4", 738, 4);
item("pants-5", 738, 5);

prop("firepit-0", 739, 0, 4);
prop("firepit-1", 739, 5, 8);

prop("well-pulley-0", 740, 0, 11);
prop("well-pulley-1", 740, 12, 23);

prop("water-trough-1", 741, 4, 7);

item("stuffed-toy-0", 742, 0);
item("stuffed-toy-1", 742, 1);

prop("strength-tester", 743);

prop("telescope"); east(744);

prop("harp", 745, 0); east(745, 1);

item("orrery-crystal", 746);

item("empowered-soul-cage", 747, 0);
item("soul-cage", 747, 1);
item("unfinished-soul-cage", 747, 2);

prop("well-of-souls", 748);

// Close enough to north for the third view...
item("alchemist-apparatus-0", 749, 0); east(749, 1); north(749, 2);
item("alchemist-apparatus-1", 749, 3); east(749, 4); north(749, 5);
item("alchemist-apparatus-2", 749, 6); east(749, 7); north(749, 8);
item("alchemist-apparatus-3", 749, 9); east(749, 10); north(749, 11);
item("alchemist-apparatus-4", 749, 12); east(749, 13); north(749, 14);
item("alchemist-apparatus-5", 749, 15); east(749, 16); north(749, 17);
item("alchemist-apparatus-6", 749, 18); east(749, 19); north(749, 20);
item("alchemist-apparatus-7", 749, 21); east(749, 22); north(749, 23);
item("alchemist-apparatus-8", 749, 24); east(749, 25); north(749, 26);
item("alchemist-apparatus-9", 749, 27); east(749, 28); north(749, 29);
item("alchemist-apparatus-10", 749, 30);
item("alchemist-apparatus-11", 749, 31);

item("music-box", 752);

// Again, not "harpie".
creature("stone-harpy", 753);

item("vial", 754);

item("caltrops", 756);

item("ethereal-ring", 759);

item("gem-0", 760, 0);
item("gem-1", 760, 1);
item("gem-2", 760, 2);
item("gem-3", 760, 3);
item("gem-4", 760, 4);
item("gem-5", 760, 5);
item("gem-6", 760, 6);
item("gem-7", 760, 7);
item("gem-8", 760, 8);
item("gem-9", 760, 9);
item("gem-10", 760, 10);
item("gem-11", 760, 11);
item("gem-12", 760, 12);
item("gem-13", 760, 13);
item("gem-14", 760, 14);

weapon("spellbook", 761);

prop("body-56", 762, 1);
prop("body-57", 762, 2);
prop("body-58", 762, 4);
prop("body-59", 762, 5);
prop("body-60", 762, 6);
prop("body-61", 762, 7);
prop("body-62", 762, 8);
prop("body-63", 762, 9);
prop("body-64", 762, 10);
prop("body-65", 762, 11);
prop("body-66", 762, 12);
prop("body-67", 762, 13);
prop("body-68", 762, 14);
prop("body-69", 762, 16);
prop("body-70", 762, 17);
prop("body-71", 762, 18);
prop("body-72", 762, 19);
prop("body-73", 762, 20);
prop("body-74", 762, 21);
prop("body-75", 762, 22);
prop("body-76", 762, 23);
prop("body-77", 762, 24);
prop("body-78", 762, 25);
prop("body-79", 762, 26);

prop("rat-race-game", 763, 0); extend(4, 0, 0, 763, 1, 2);
// TODO: Figure out how 764 fits on the rest...

prop("planet-britannia", 765);

prop("moon-trammel", 766);

prop("small-orrery", 767);

prop("energy-field", 768);

item("smokebomb", 769);

prop("orrery-viewer", 770);

weapon("rudyoms-wand", 771);

item("honey", 772);

prop("red-moongate", 776); south(779);

prop("blue-moongate", 777);

prop("body-smith-the-horse", 778, 1);
prop("body-unicorn", 778, 2);
prop("body-hydra", 778, 3);
prop("body-cyclops", 778, 4); alt(778, 6);
prop("body-troll", 778, 5); alt(778, 10);
prop("body-dragon", 778, 7);
prop("body-drake", 778, 8);
prop("body-horse", 778, 9);
prop("body-cow", 778, 11);
prop("body-deer", 778, 12);

tree("tree-24", 783, 0);
tree("tree-25", 783, 1);
tree("tree-26", 783, 2);

item("orb-of-the-moons", 785);

item("vortex-cube", 786);

prop("lever", 787, 0, 1); east(787, 2, 3);

prop("switch", 788, 0, 1); east(788, 2, 3);

prop("sword-in-a-stone", 789);

item("invisibility-dust", 790);

weapon("magicians-wand", 792);

creature("draft-horse", 796);

item("scroll-0", 797, 0);
item("scroll-1", 797, 1);
item("scroll-2", 797, 2);
item("scroll-3", 797, 3);

item("sealed-box", 798);
item("unsealed-box", 799);

item("chest", 800, 0); east(800, 1);

// Here's one with vertical offset
tree("tree-top", 826, 1);
extend(0, 0, -5, 826, 0);
