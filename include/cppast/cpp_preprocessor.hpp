// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_CPP_PREPROCESSOR_HPP_INCLUDED
#define CPPAST_CPP_PREPROCESSOR_HPP_INCLUDED

#include <type_safe/optional.hpp>

#include <cppast/cpp_entity.hpp>
#include <cppast/cpp_file.hpp>

namespace cppast
{
    /// A [cppast::cpp_entity]() modelling a macro definition.
    class cpp_macro_definition final : public cpp_entity
    {
    public:
        /// \returns A newly built macro definition.
        /// \notes It is not meant to be registered in the [cppast::cpp_entity_index](),
        /// as no other [cppast::cpp_entity]() can refer to it.
        static std::unique_ptr<cpp_macro_definition> build(
            std::string name, type_safe::optional<std::string> parameters, std::string replacement)
        {
            return std::unique_ptr<cpp_macro_definition>(
                new cpp_macro_definition(std::move(name), std::move(parameters),
                                         std::move(replacement)));
        }

        /// \returns The replacement text of the macro.
        const std::string& replacement() const noexcept
        {
            return replacement_;
        }

        /// \returns Whether or not it is a function like macro.
        bool is_function_like() const noexcept
        {
            return parameters_.has_value();
        }

        /// \returns The parameters of the macro, as the string spelled out in the source code.
        /// \notes It has none if it is not a function like macro.
        const type_safe::optional<std::string>& parameters() const noexcept
        {
            return parameters_;
        }

    private:
        cpp_entity_kind do_get_entity_kind() const noexcept override;

        cpp_macro_definition(std::string name, type_safe::optional<std::string> parameters,
                             std::string replacement)
        : cpp_entity(std::move(name)),
          parameters_(std::move(parameters)),
          replacement_(std::move(replacement))
        {
        }

        type_safe::optional<std::string> parameters_;
        std::string                      replacement_;
    };

    /// The kind of [cppast::cpp_include_directive]().
    enum class cpp_include_kind
    {
        system, //< An `#include <...>`.
        local,  //< An `#include "..."`.
    };

    /// A [cppast::cpp_entity]() modelling an `#include`.
    class cpp_include_directive final : public cpp_entity
    {
    public:
        /// \returns A newly built include directive.
        /// \notes It is not meant to be registered in the [cppast::cpp_entity_index](),
        /// as no other [cppast::cpp_entity]() can refer to it.
        static std::unique_ptr<cpp_include_directive> build(const cpp_file_ref& target,
                                                            cpp_include_kind    kind)
        {
            return std::unique_ptr<cpp_include_directive>(new cpp_include_directive(target, kind));
        }

        /// \returns A reference to the [cppast::cpp_file]() it includes.
        cpp_file_ref target() const noexcept
        {
            return cpp_file_ref(target_, name());
        }

        /// \returns The kind of include it is.
        cpp_include_kind include_kind() const noexcept
        {
            return kind_;
        }

    private:
        cpp_entity_kind do_get_entity_kind() const noexcept override;

        cpp_include_directive(const cpp_file_ref& target, cpp_include_kind kind)
        : cpp_entity(target.name()), target_(target.id()), kind_(kind)
        {
        }

        cpp_entity_id    target_;
        cpp_include_kind kind_;
    };
} // namespace cppast

#endif // CPPAST_CPP_PREPROCESSOR_HPP_INCLUDED