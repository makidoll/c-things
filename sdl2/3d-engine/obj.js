if (!process.argv[2]) {
	console.log("node obj [.obj]");
	return;
}

var fs = require("fs");

let out =
	"#ifndef OBJ_H\n"+
	"#define OBJ_H\n";

let verts = "Vec3f verts[]={";
let faces = "Vec3i faces[]={";

let map = fs.readFileSync(process.argv[2], "utf8");
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

out += verts.slice(0, -1)+"};\n"+faces.slice(0, -1)+"};\n"+
	"#endif";

fs.writeFileSync("obj.h", out);