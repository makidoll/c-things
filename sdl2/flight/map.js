var fs = require("fs");

let verts = "Vec3f verts[]={";
let faces = "Vec3i faces[]={";

let map = fs.readFileSync("./map.obj", "utf8");
map.split("\n").forEach((line, i) => {
	
	args = line.split(" ");

	switch(args[0].toLowerCase()) {
		case "v": verts+="{"+args[1]+","+args[2]+","+args[3]+"},"; break;
		case "f": 
			faces+="{"+
				(parseInt(args[1])-1)+","+
				(parseInt(args[2])-1)+","+
				(parseInt(args[3])-1)+"},";
			break;
	}

});

let out = verts.slice(0, -1)+"};\n"+faces.slice(0, -1)+"};";

fs.writeFileSync("map.c", out);