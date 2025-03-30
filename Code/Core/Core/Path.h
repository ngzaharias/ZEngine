#pragma once

#include "Core/String.h"
#include "Core/StringView.h"

namespace str
{
	enum class EPath
	{
		AppData,
		Assets,
		Config,
		Executable,
		Levels,
		ThirdParty,
		WorkingDir,
	};

	class Path final
	{
	public:
		Path();
		Path(str::String&& value);
		Path(const char* value);
		Path(const str::String& value);
		Path(const str::StringView& value);
		Path(const str::EPath type);
		template<typename... TStringViews>
		Path(const str::EPath type, const TStringViews& ...args);
		template<typename... TStringViews>
		Path(const str::Path& value, const TStringViews& ...args);

		bool operator==(const str::Path& rhs) const;
		bool operator==(const str::StringView& rhs) const;
		bool operator!=(const str::Path& rhs) const;
		bool operator!=(const str::StringView& rhs) const;

		bool operator<(const str::Path& rhs) const;

		void operator=(str::String&& rhs);
		void operator=(const str::Path& rhs);
		void operator=(const str::String& rhs);
		void operator=(const str::StringView& rhs);

		void operator+=(const str::StringView& rhs);

		operator str::StringView() const { return m_Value; }

		bool HasExtension() const;
		bool IsDirectory() const;
		bool IsEmpty() const { return m_Value.empty(); }

		const char* ToChar() const { return m_Value.c_str(); }
		str::StringView ToView() const { return m_Value; }

		///		/My/Example/Directory/Foo.bar
		///		++++++++++++++++++++++
		str::StringView GetDirectory() const;

		///		/My/Example/Directory/Foo.bar
		///								 ++++
		str::StringView GetFileExtension() const;

		///		/My/Example/Directory/Foo.bar
		///							  +++++++
		str::StringView GetFileName() const;

		///		/My/Example/Directory/Foo.bar
		///							  +++
		str::StringView GetFileNameNoExtension() const;

		///		/My/Example/Directory/
		///			+++++++
		///		/My/Example/Directory/Foo.bar
		///					+++++++++
		str::StringView GetParent() const;

		///		/My/Example/Directory/
		///				    +++++++++
		///		/My/Example/Directory/Foo.bar
		///							  +++++++
		str::StringView GetStem() const;

		void Clear();

	private:
		str::String m_Value;
	};

	/// \brief Returns the full path of the internal path type.
	const str::Path& GetPath(const EPath type);

	/// \brief Sets the full path of the internal path type.
	void SetPath(const EPath type, const str::Path& value);
}

#include "Path.inl"