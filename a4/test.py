

accept = "*/*;q=0.5, image/png;q=0.6"

types = accept.split(sep= ", ")

types_dict = {}
for type_ in types:
	try:
		if ";" in type_:
			key, value = type_.split(sep=";", maxsplit=1)
			variable, value = value.split(sep="=", maxsplit=1)
			types_dict.update({key: value})
		else:
			types_dict.update({type_: "1.1"})
	except:
		print("Here")

types_subtypes = {"*": "*", "image": ["jpeg", "png", "svg+xml"], "text": ["html", "plain", "csv", "css"], "application": ["xhtml+xml", "xml"]}
accepted_list = []
for type_subtype in types_dict.keys():
	MIME_type, MIME_subtype = type_subtype.split(sep="/")
	if MIME_type in types_subtypes:
		if MIME_subtype in types_subtypes[MIME_type]:
			#Make a list of all matches --> Sorted List
			accepted_list.append(MIME_type+"/"+MIME_subtype)
		else:
			next
	else:
		next
if accepted_list == []:
	print(406)
else:
	final_dict = {}
	for elem in accepted_list:
		value = types_dict[elem]
		final_dict.update({elem: value})
	sorted_final_list_tuples = sorted(final_dict.items(), key= lambda x:x[1], reverse=True)
	
	# Search for file ending in sorted order from URL and self 
	for best_option in sorted_final_list_tuples:
		print(best_option)
		
		#if os.path.exists(self.url + "." + best_option[0]):
		#	self.url = self.url+"."+best_option[0]
		#else:
		#		next