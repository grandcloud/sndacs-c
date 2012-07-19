/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */
#include <string.h>


#include "snda_ecs_sdk.h"
#include "snda_ecs_common_util.h"
#include "snda_ecs_constants.h"




/**
 * key sorted extensionfilename->contenttype map
 */
static const SNDAECSPair s_snda_ecs_content_type_map[] =
{
		{"3gp", "video/3gpp"},
		{"ai", "application/postscript"},
		{"aif", "audio/x-aiff"},
		{"aifc", "audio/x-aiff"},
		{"aiff", "audio/x-aiff"},
		{"asc", "text/plain"},
		{"atom", "application/atom+xml"},
		{"au", "audio/basic"},
		{"avi", "video/x-msvideo"},
		{"bcpio", "application/x-bcpio"},
		{"bin", "application/octet-stream"},
		{"bmp", "image/bmp"},
		{"cdf", "application/x-netcdf"},
		{"cgm", "image/cgm"},
		{"class", "application/octet-stream"},
		{"cpio", "application/x-cpio"},
		{"cpt", "application/mac-compactpro"},
		{"csh", "application/x-csh"},
		{"css", "text/css"},
		{"dcr", "application/x-director"},
		{"dif", "video/x-dv"},
		{"dir", "application/x-director"},
		{"djv", "image/vnd.djvu"},
		{"djvu", "image/vnd.djvu"},
		{"dll", "application/octet-stream"},
		{"dmg", "application/octet-stream"},
		{"dms", "application/octet-stream"},
		{"doc", "application/msword"},
		{"dtd", "application/xml-dtd"},
		{"dv", "video/x-dv"},
		{"dvi", "application/x-dvi"},
		{"dxr", "application/x-director"},
		{"eps", "application/postscript"},
		{"etx", "text/x-setext"},
		{"exe", "application/octet-stream"},
		{"ez", "application/andrew-inset"},
		{"flv", "video/x-flv"},
		{"gif", "image/gif"},
		{"gram", "application/srgs"},
		{"grxml", "application/srgs+xml"},
		{"gtar", "application/x-gtar"},
		{"gz", "application/x-gzip"},
		{"hdf", "application/x-hdf"},
		{"hqx", "application/mac-binhex40"},
		{"htm", "text/html"},
		{"html", "text/html"},
		{"ice", "x-conference/x-cooltalk"},
		{"ico", "image/x-icon"},
		{"ics", "text/calendar"},
		{"ief", "image/ief"},
		{"ifb", "text/calendar"},
		{"iges", "model/iges"},
		{"igs", "model/iges"},
		{"jnlp", "application/x-java-jnlp-file"},
		{"jp2", "image/jp2"},
		{"jpe", "image/jpeg"},
		{"jpeg", "image/jpeg"},
		{"jpg", "image/jpeg"},
		{"js", "application/x-javascript"},
		{"kar", "audio/midi"},
		{"latex", "application/x-latex"},
		{"lha", "application/octet-stream"},
		{"lzh", "application/octet-stream"},
		{"m3u", "audio/x-mpegurl"},
		{"m4a", "audio/mp4a-latm"},
		{"m4p", "audio/mp4a-latm"},
		{"m4u", "video/vnd.mpegurl"},
		{"mac", "image/x-macpaint"},
		{"man", "application/x-troff-man"},
		{"mathml", "application/mathml+xml"},
		{"me", "application/x-troff-me"},
		{"mesh", "model/mesh"},
		{"mid", "audio/midi"},
		{"midi", "audio/midi"},
		{"mif", "application/vnd.mif"},
		{"mov", "video/quicktime"},
		{"movie", "video/x-sgi-movie"},
		{"mp2", "audio/mpeg"},
		{"mp3", "audio/mpeg"},
		{"mp4", "video/mp4"},
		{"mpe", "video/mpeg"},
		{"mpeg", "video/mpeg"},
		{"mpg", "video/mpeg"},
		{"mpga", "audio/mpeg"},
		{"ms", "application/x-troff-ms"},
		{"msh", "model/mesh"},
		{"mxu", "video/vnd.mpegurl"},
		{"nc", "application/x-netcdf"},
		{"oda", "application/oda"},
		{"ogg", "application/ogg"},
		{"pbm", "image/x-portable-bitmap"},
		{"pct", "image/pict"},
		{"pdb", "chemical/x-pdb"},
		{"pdf", "application/pdf"},
		{"pgm", "image/x-portable-graymap"},
		{"pgn", "application/x-chess-pgn"},
		{"pic", "image/pict"},
		{"pict", "image/pict"},
		{"png", "image/png"},
		{"pnm", "image/x-portable-anymap"},
		{"pnt", "image/x-macpaint"},
		{"pntg", "image/x-macpaint"},
		{"ppm", "image/x-portable-pixmap"},
		{"ppt", "application/vnd.ms-powerpoint"},
		{"ps", "application/postscript"},
		{"qt", "video/quicktime"},
		{"qti", "image/x-quicktime"},
		{"qtif", "image/x-quicktime"},
		{"ra", "audio/x-pn-realaudio"},
		{"ram", "audio/x-pn-realaudio"},
		{"ras", "image/x-cmu-raster"},
		{"rdf", "application/rdf+xml"},
		{"rgb", "image/x-rgb"},
		{"rm", "application/vnd.rn-realmedia"},
		{"roff", "application/x-troff"},
		{"rtf", "text/rtf"},
		{"rtx", "text/richtext"},
		{"sgm", "text/sgml"},
		{"sgml", "text/sgml"},
		{"sh", "application/x-sh"},
		{"shar", "application/x-shar"},
		{"silo", "model/mesh"},
		{"sit", "application/x-stuffit"},
		{"skd", "application/x-koan"},
		{"skm", "application/x-koan"},
		{"skp", "application/x-koan"},
		{"skt", "application/x-koan"},
		{"smi", "application/smil"},
		{"smil", "application/smil"},
		{"snd", "audio/basic"},
		{"so", "application/octet-stream"},
		{"spl", "application/x-futuresplash"},
		{"src", "application/x-wais-source"},
		{"sv4cpio", "application/x-sv4cpio"},
		{"sv4crc", "application/x-sv4crc"},
		{"svg", "image/svg+xml"},
		{"swf", "application/x-shockwave-flash"},
		{"t", "application/x-troff"},
		{"tar", "application/x-tar"},
		{"tcl", "application/x-tcl"},
		{"tex", "application/x-tex"},
		{"texi", "application/x-texinfo"},
		{"texinfo", "application/x-texinfo"},
		{"tif", "image/tiff"},
		{"tiff", "image/tiff"},
		{"tr", "application/x-troff"},
		{"tsv", "text/tab-separated-values"},
		{"txt", "text/plain"},
		{"ustar", "application/x-ustar"},
		{"vcd", "application/x-cdlink"},
		{"vrml", "model/vrml"},
		{"vxml", "application/voicexml+xml"},
		{"wav", "audio/x-wav"},
		{"wbmp", "image/vnd.wap.wbmp"},
		{"wbxml", "application/vnd.wap.wbxml"},
		{"wml", "text/vnd.wap.wml"},
		{"wmlc", "application/vnd.wap.wmlc"},
		{"wmls", "text/vnd.wap.wmlscript"},
		{"wmlsc", "application/vnd.wap.wmlscriptc"},
		{"wmv", "video/x-ms-wmv"},
		{"wrl", "model/vrml"},
		{"xbm", "image/x-xbitmap"},
		{"xht", "application/xhtml+xml"},
		{"xhtml", "application/xhtml+xml"},
		{"xls", "application/vnd.ms-excel"},
		{"xml", "application/xml"},
		{"xpm", "image/x-xpixmap"},
		{"xsl", "application/xml"},
		{"xslt", "application/xslt+xml"},
		{"xul", "application/vnd.mozilla.xul+xml"},
		{"xwd", "image/x-xwindowdump"},
		{"xyz", "chemical/x-xyz"},
		{"zip", "application/zip"}
};

static char* snda_ecs_get_file_extension_name(const char* filename, char* extensionname) {
	int j = 0;
	int i = strlen(filename) - 1;
	for (; i >= 0; --i) {
		if (filename[i] == '.') {
			break;
		}

		if (++j >= S_SNDA_ECS_MAX_EXTENSION_NAME_LEN) {
			extensionname[0] = '\0';
			return extensionname;
		}
	}


	if (i > 0) {
		j = 0;
		for (++i; i < strlen(filename); ++i) {
			extensionname[j++] = tolower(filename[i]);
		}
		extensionname[j] = '\0';
	} else {
		extensionname[0] = '\0';
	}
	return extensionname;
}

static int snda_ecs_search_comparator(const void* left, const void* right) {
	return strcmp((const char*)left, (const char*) (((SNDAECSPair*)right)->key));
}

char* snda_ecs_get_content_type(const char* filename, char* contenttype) {
	sprintf(contenttype, "%s", snda_ecs_default_content_type);

	char extensionname[S_SNDA_ECS_MAX_EXTENSION_NAME_LEN];
	snda_ecs_get_file_extension_name(filename, extensionname);

	if (strlen(extensionname)) {
		SNDAECSPair* match = (SNDAECSPair*)bsearch(extensionname,
				s_snda_ecs_content_type_map,
				sizeof(s_snda_ecs_content_type_map)/sizeof(SNDAECSPair),
				sizeof(SNDAECSPair),
				snda_ecs_search_comparator);
		if (match) {
			sprintf(contenttype, "%s", match->value);
		}
	}
	return contenttype;
}


SNDAECSCommonPrefix* snda_ecs_to_common_prefix(const xmlNodePtr cpelementnode)
{
	if (!snda_ecs_is_xml_element_node_with_name(cpelementnode, "CommonPrefixes")) {
		return NULL;
	}

	if (!cpelementnode ||
		!(cpelementnode->xmlChildrenNode) ||
		!(cpelementnode->xmlChildrenNode->children) ||
		!(cpelementnode->xmlChildrenNode->children->content)) {
		return NULL;
	}

	SNDAECSCommonPrefix* node = snda_ecs_init_common_prefix();
	char* value = (char*)(cpelementnode->xmlChildrenNode->children->content);
	node->commonprefix = (char*)calloc(strlen(value) + 1, sizeof(char));
	strcpy(node->commonprefix, value);

	return node;
}
