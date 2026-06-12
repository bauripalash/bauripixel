set(BAURIPIXEL_SRC_FILES
	"${CMAKE_CURRENT_LIST_DIR}/app.c"
	"${CMAKE_CURRENT_LIST_DIR}/options.c"
	"${CMAKE_CURRENT_LIST_DIR}/theme.c"
	"${CMAKE_CURRENT_LIST_DIR}/components/panel.c"
	"${CMAKE_CURRENT_LIST_DIR}/widgets/colorpanel.c"
	
	# External
	"${CMAKE_CURRENT_LIST_DIR}/external/raylib/raygui_impl.c"
	"${CMAKE_CURRENT_LIST_DIR}/external/stb/stb_ds_impl.c"
)

set(BAURIPIXEL_HEADER_FILES 
	"${CMAKE_CURRENT_LIST_DIR}/include/alloc.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/btypes.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/colors.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/components.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/defaults.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/options.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/theme.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/widget.h"
	"${CMAKE_CURRENT_LIST_DIR}/external/raylib/raygui.h"
)

set(BAURIPIXEL_MAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/main.c")
