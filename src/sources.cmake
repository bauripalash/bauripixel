set(BAURIPIXEL_SRC_FILES
	"${CMAKE_CURRENT_LIST_DIR}/app.c"
	"${CMAKE_CURRENT_LIST_DIR}/components/panel.c"
	"${CMAKE_CURRENT_LIST_DIR}/options.c"
	"${CMAKE_CURRENT_LIST_DIR}/theme.c"
	"${CMAKE_CURRENT_LIST_DIR}/widgets/colorpanel.c"
	"${CMAKE_CURRENT_LIST_DIR}/widgets/drawarea.c"
	
	# External
	"${CMAKE_CURRENT_LIST_DIR}/external/raylib/raygui_impl.c"
	"${CMAKE_CURRENT_LIST_DIR}/external/stb/stb_ds_impl.c"
)

set(BAURIPIXEL_HEADER_FILES 
	"${CMAKE_CURRENT_LIST_DIR}/alloc.h"
	"${CMAKE_CURRENT_LIST_DIR}/btypes.h"
	"${CMAKE_CURRENT_LIST_DIR}/colors.h"
	"${CMAKE_CURRENT_LIST_DIR}/components.h"
	"${CMAKE_CURRENT_LIST_DIR}/defaults.h"
	"${CMAKE_CURRENT_LIST_DIR}/options.h"
	"${CMAKE_CURRENT_LIST_DIR}/theme.h"
	"${CMAKE_CURRENT_LIST_DIR}/widget.h"
	"${CMAKE_CURRENT_LIST_DIR}/external/raylib/raygui.h"
)

set(BAURIPIXEL_MAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/main.c")
