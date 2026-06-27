#
# src/CMakeLists.txt - CMakeLists For 'src' Directory
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


include("${CMAKE_CURRENT_LIST_DIR}/sources.cmake")

target_sources(${PROJECT_NAME}
	PRIVATE 
	${BAURIPIXEL_SRC_FILES} 
	${BAURIPIXEL_MAIN_FILE} 
	${BAURIPIXEL_HEADER_FILES}
)


