#pragma once

#include <type_traits>
#include <utility>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <variant>
#include <exception>

namespace tag {

inline constexpr struct Exception {
} kEx;

}  // namespace tag

template <class T>
class Try {
public:
    template <class TT>
    Try(TT&& value) : var_() {
        var_.template emplace<1>(std::forward<TT>(value));
    }

    template <class E>
    Try(tag::Exception, E&& exception) : var_(std::make_exception_ptr(std::forward<E>(exception))) {
    }
    template <>
    Try(tag::Exception, std::exception_ptr&& eptr) : var_() {
        var_.template emplace<2>(std::move(eptr));
    }

    Try() : var_() {
    }

    Try(const Try&) = delete;
    Try(Try&&) = delete;

    void Throw() const {
        if (IsFailed()) {
            std::rethrow_exception(std::get<2>(var_));
        } else {
            throw std::runtime_error("No exception");
        }
    }

    const T& Value() const {
        if (IsFailed()) {
            std::rethrow_exception(std::get<2>(var_));
        }
        if (IsEmpty()) {
            throw std::runtime_error("Object is empty");
        }
        return std::get<1>(var_);
    }

    bool IsEmpty() const {
        return var_.index() == 0;
    }
    bool IsFailed() const {
        return var_.index() == 2;
    }

private:
    std::variant<std::monostate, T, std::exception_ptr> var_;
};

template <typename TT>
Try(TT&&) -> Try<std::remove_reference_t<TT>>;

template <>
class Try<void> {
public:
    Try() : var_() {
    }

    template <class E>
    Try(tag::Exception, E&& exception) : var_(std::make_exception_ptr(std::forward<E>(exception))) {
    }

    template <>
    Try(tag::Exception, std::exception_ptr&& eptr) : var_() {
        var_.template emplace<1>(std::move(eptr));
    }

    Try(const Try&) = delete;
    Try(Try&&) = delete;

    void Throw() const {
        if (IsFailed()) {
            std::rethrow_exception(std::get<std::exception_ptr>(var_));
        } else {
            throw std::runtime_error("No exception");
        }
    }

    bool IsEmpty() const {
        return std::holds_alternative<std::monostate>(var_);
    }
    bool IsFailed() const {
        return std::holds_alternative<std::exception_ptr>(var_);
    }

private:
    std::variant<std::monostate, std::exception_ptr> var_;
};

template <class Function, class... Args>
auto TryRun(Function func, Args&&... args) {
    using ReturnType = decltype(func(std::forward<Args>(args)...));

    try {
        if constexpr (std::is_same_v<ReturnType, void>) {
            func(std::forward<Args>(args)...);
            return Try<void>();
        } else {
            return Try<ReturnType>(func(std::forward<Args>(args)...));
        }

    } catch (const std::exception& except) {
        return Try<ReturnType>(tag::kEx, std::current_exception());

    } catch (const char* cstr) {
        return Try<ReturnType>(tag::kEx, cstr);

    } catch (const int& err_code) {
        return Try<ReturnType>(tag::kEx, std::system_error(err_code, std::generic_category()));

    } catch (...) {
        return Try<ReturnType>(tag::kEx, std::runtime_error("Unknown exception"));
    }
}
