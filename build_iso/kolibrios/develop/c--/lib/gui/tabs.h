#ifndef TAB_PADDING
#define TAB_PADDING 15
#endif

#define TAB_HEIGHT 28
#define NAME_SIZE 64

:struct _tabs
{
	int x,y,w;
	int base_id;

	int active_tab;
	char names[640];
	int count;
	dword events[10];

	int click();
	void draw();
	void draw_active_tab();
	void add();

	dword draw_button();
};

:void _tabs::draw()
{
	int i, xx=x;

	if (w) {
		DrawBar(x+1,y+0+TAB_HEIGHT,w,1, sc.work_graph);
		DrawBar(x+1,y+1+TAB_HEIGHT,w,1, sc.work_light);		
	}

	for (i=0; i<count; i++) {
		xx += draw_button(xx + TAB_PADDING, i, i*NAME_SIZE + #names) + TAB_PADDING;
	}
}

:void _tabs::draw_active_tab()
{
	events[active_tab]();
}

:void _tabs::add(dword text, event)
{
	strcpy(count*NAME_SIZE + #names, text);
	events[count] = event;
	count++;
}

:dword _tabs::draw_button(dword xx, _id, text)
{
	dword col_bg, col_text;
	dword ww=strlen(text)*8;

	if (_id==active_tab)
	{
		col_bg = 0xE44C9C;
		col_text = sc.work_text;
	}
	else
	{
		col_bg = 0xC3A1B7;
		col_text = MixColors(sc.work, sc.work_text, 120);
	} 
	DefineHiddenButton(xx-2,y, ww-1+4,TAB_HEIGHT-1, _id + base_id);
	WriteText(xx, y+6, 0x90, col_text, text);
	DrawBar(xx, y+TAB_HEIGHT-3, ww, 3, col_bg);
	return ww;
}

:int _tabs::click(int _id)
{
	if (_id < base_id) || (_id > base_id + count) || (_id == active_tab) {
		return false; 
	}
	active_tab = _id - base_id;
	events[active_tab]();
	return true;		
}