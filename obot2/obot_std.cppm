module;

#ifdef __clang__
#include <string_view>
#endif

export module obot_std;

export import <bit>;
export import <cstdint>;
export import <cstring>;
export import <charconv>;
#ifndef __clang__
export import <string>;
import <string_view>;
#endif
export namespace std {
    using std::basic_string_view;
    using std::string_view;
}
// import <string>;
// export namespace std {
//     using std::basic_string;
//     using std::char_traits;
//     using std::operator+;
//     using std::string;
//     using std::to_string;
// }
