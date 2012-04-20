/*
    Mosh: the mobile shell
    Copyright 2012 Keith Winstein

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TERMINALUSERINPUT_HPP
#define TERMINALUSERINPUT_HPP

#include <string>
#include "parseraction.h"

namespace Terminal {
  class UserInput {
  public:
    enum UserInputState {
      Ground,
      ESC,
      SS3
    };

  private:
    UserInputState state;

  public:
    UserInput()
      : state( Ground )
    {}

    std::string input( const Parser::UserByte *act,
		       bool application_mode_cursor_keys );

    bool operator==( const UserInput &x ) const { return state == x.state; }
  };
}

#endif
