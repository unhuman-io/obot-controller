module;

#include <algorithm>
#include <bit>
#include <cstdint>
#include <string>
#include <string_view>
#include <cstring>
#include <charconv>
#include <system_error>
#include <cstddef>

export module obot_std;

// algorithm
export namespace std {
  using std::min;
}

// export import <bit>;
export namespace std {
  using std::bit_cast;
}


// export import <cstdint>;
export {
    using std::int8_t;
    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::intptr_t;
    using std::uint8_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::uintptr_t;
}

export namespace std {
    using std::int8_t;
    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::intptr_t;
    using std::uint8_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::uintptr_t;
}


export namespace std {
  using std::basic_string_view;
  using std::operator==;
  using std::operator<=>;

  using std::operator<<;

  using std::string_view;

  inline namespace literals {
    inline namespace string_view_literals {
      using std::literals::string_view_literals::operator""sv;
    } 
  }
}


// export import <cstring>;
export {
    using ::size_t;
}

export namespace std {
  using std::memchr;
  using std::memcmp;
  using std::memcpy;
  using std::memmove;
  using std::memset;
  using std::strcat;
  using std::strchr;
  using std::strcmp;
  using std::strcoll;
  using std::strcpy;
  using std::strcspn;
  using std::strerror;
  using std::strlen;
  using std::strncat;
  using std::strncmp;
  using std::strncpy;
  using std::strpbrk;
  using std::strrchr;
  using std::strspn;
  using std::strstr;
  using std::strtok;
  using std::strxfrm;
}

// export import <charconv>;
export namespace std {
  using std::chars_format;

  using std::operator&;
  using std::operator&=;
  using std::operator^;
  using std::operator^=;
  using std::operator|;
  using std::operator|=;
  using std::operator~;

  using std::to_chars_result;

  using std::to_chars;
  using std::from_chars_result;

  using std::from_chars;
}

// import <string>;
export namespace std {
    using std::basic_string;
    using std::char_traits;
    using std::operator+;
    using std::string;
    using std::to_string;
}

// system error
export namespace std {
  using std::errc;

  using std::operator==;
  using std::operator<=>;
}
