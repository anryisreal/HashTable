#include <string>

namespace Hash {
    class HashException : public std::exception {
        protected:
            std::string msg;
        public:
            HashException(const char* er_msg) : msg{er_msg} {}
            HashException(const HashException& other) : msg{other.msg} {}
            const char* what () const noexcept override {
                return msg.c_str();
            }
    };

    class HE_IndexError : public HashException {
        private:
            void writemsg(int& capacity) noexcept {
                msg += std::to_string(capacity);
            }
        public:
            HE_IndexError(int capacity) : HashException("IndexError: index goes beyond the size limits: The requested index exceeds - ") {
                writemsg(capacity);
            }
    };
    
    class HE_InitialError : public HashException {
        public:
            HE_InitialError() : HashException("InitialError: an attempt to query a value from an uninitialized hash-table") {}
    };

    class HE_SizeError: public HashException {
        public:
            HE_SizeError() : HashException("SizeError: There is no place to insert an element") { }
    };
}