#
# sources.cmake - CMake Listing of Source Files
# Copyright (C) <year>  <name of author>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

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
