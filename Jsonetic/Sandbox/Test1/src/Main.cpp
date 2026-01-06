#include <iostream>
#include <string>
#include <map>

#include <Jsonetic/Jsonetic.h>

int main()
{
	std::string source = ""
		"{																													"
		"    \"glossary\": {																								"
		"       \"title\": \"example glossary\",																			"
		"		\"GlossDiv\": {																								"
		"           \"title\": \"Stand\",																					"
		"			\"GlossList\": {																						"
		"                \"GlossEntry\": {																					"
		"                    \"ID\": \"SGML\",																				"
		"					\"SortAs\": \"SGML\",																			"
		"					\"GlossTerm\": \"Standard Generalized Markup Language\",										"
		"					\"Acronym\": \"SGML\",																			"
		"					\"Abbrev\": \"ISO 8879:1986\",																	"
		"					\"GlossDef\": {																					"
		"                       \"para\": \"A meta - markup language, used to create markup languages such as DocBook.\",	"
		"						\"GlossSeeAlso\": [\"GML\", \"XML\"]														"
		"                    },																								"
		"					\"GlossSee\": \"markup\"																		"
		"                }																									"
		"            }																										"
		"        }																											"
		"    }																												"
		"}																													";

	std::string source2 = "{\"name\":\"John\", \"age\" : false, \"car\" :null }";

	jsonc::Object json = jsonc::Parse(source);
	jsonc::Object tree = jsonc::ParseFile("D:/dev/Monk/Monk/res/this_tree_is_growing/scene.gltf");

	std::cout << tree << std::endl;

	return 0;
}