#ifdef _FORMAT_SINGLE_FILE
#define _HREF(topicfile,linkedtext) \
  <a href=\"_HELP_FILE#topicfile\">linkedtext</a>
#else
#define _HREF(topicfile,linkedtext) \
  <a href=\"topicfile.htm\">linkedtext</a>
#endif

#define PAGE_END \
\
</FONT></BODY></HTML>


#define MENUITEM(item_name,item_description) \
<TR><TD WIDTH=30>\
<TD VALIGN=TOP WIDTH=158>\
<FONT FACE="Arial, Lucida, Helvetica">\
<B>item_name</B>\
</FONT></TD>\
<TD VALIGN=TOP>\
<FONT FACE="Arial, Lucida, Helvetica">\
item_description\
</FONT></TD></TR>\



#define MENURULE \
<TR><TD WIDTH=30></TD><TD COLSPAN=2>\
<FONT FACE="Arial, Lucida, Helvetica">\
<HR COLOR=#C0C0C0>\
</FONT></TD>\
</TR>


