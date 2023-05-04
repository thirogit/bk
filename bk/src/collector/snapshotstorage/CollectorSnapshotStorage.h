#pragma once

#include "../../localstorage/LocalStorage.h"
#include "../data/DownloadSnapshot.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/filesystem.hpp>
#include <boost/random/mersenne_twister.hpp>
#include "../data/DownloadSnapshotHeader.h"

namespace fs = boost::filesystem;

#include <boost/regex.hpp>
#include <boost/exception/all.hpp>

struct dir_filter_iter
	: public boost::iterator_facade<
	dir_filter_iter,
	boost::filesystem::path,
	boost::forward_traversal_tag,
	boost::filesystem::path
	>
{
	using path = boost::filesystem::path;
	using impl_type = boost::filesystem::directory_iterator;


	dir_filter_iter() :impl_() {}
	dir_filter_iter(path p, boost::wregex rgx) :impl_(std::move(p)), rgx_(std::move(rgx)) {
		
		if (!fs::is_directory(p)) {
			BOOST_THROW_EXCEPTION(
				boost::enable_error_info(std::domain_error("not a dir"))
				<< boost::errinfo_file_name(p.string()));
		}
		if (impl_ != impl_type())
		{
			while (impl_ != impl_type() && (fs::is_directory(impl_->path()) || !boost::regex_match(impl_->path().filename().wstring(), rgx_))) impl_++;
		}
	}
private:
	friend class boost::iterator_core_access;
	bool equal(const dir_filter_iter& that)const {
		return this->impl_ == that.impl_;
	}
	void increment() {
		assert(impl_ != impl_type());
		for (;;) {
			++impl_;
			if (impl_ == impl_type())
				break;

			if (fs::is_directory(impl_->path()))
				continue;

			std::wstring s(impl_->path().filename().wstring());
			if (boost::regex_match(s, rgx_)) {
				break;
			}
		}
	}
	path dereference()const {
		assert(impl_ != impl_type());
		return *impl_;
	}
	impl_type impl_;
	boost::wregex rgx_;
};

struct dir_filter_iter_maker {
	using value_type = dir_filter_iter;

	explicit dir_filter_iter_maker(boost::wregex rgx) :rgx_(rgx) {}

	value_type make()const {
		return value_type();
	}
	value_type make(boost::filesystem::path p)const {
		return value_type(std::move(p), rgx_);
	}
	template<typename... Args>
	auto operator()(Args&&... args)->decltype(make(args...)) {
		return this->make(std::forward<Args>(args)...);
	}
private:
	boost::wregex rgx_;
};


class snapshot_iterator
	: public boost::iterator_facade<
	snapshot_iterator
	, std::wstring
	, boost::forward_traversal_tag
	, std::wstring
	>
{
public:
	snapshot_iterator()	 {}

	explicit snapshot_iterator(dir_filter_iter& it)	: m_it(it)
	{
		if (it != dir_filter_iter())
		{
			m_current = m_it->filename().stem().wstring();
		}
	}

private:
	friend class boost::iterator_core_access;

	void increment() 
	{ 
		m_it++; 
		if (m_it != dir_filter_iter())
		{
			m_current = m_it->filename().wstring();
		}
		else
		{
			m_current.clear();
		}
	}

	bool equal(snapshot_iterator const& other) const
	{
		return this->m_it == other.m_it;
	}

	std::wstring dereference() const { return m_current; }

	dir_filter_iter m_it;
	std::wstring m_current;
	
};


typedef boost::mt19937 generator_type;

class CollectorSnapshotStorage : LocalStorage
{
public:
	CollectorSnapshotStorage();

	uint32_t Save(const DownloadSnapshot& snapshot);
	std::unique_ptr<DownloadSnapshot> Load(uint32_t id);
	std::unique_ptr<DownloadSnapshot> Load(const std::wstring& id);
	std::unique_ptr<DownloadSnapshotHeader> GetHeader(uint32_t id);
	std::unique_ptr<DownloadSnapshotHeader> GetHeader(const std::wstring& id);
	void Delete(uint32_t id);
	snapshot_iterator List();

protected:
	void SaveJson(const std::wstring& file, const json::value& value);
	std::wstring GetFileName(uint32_t id) const;
	std::wstring GetSnapshotStorageDir();
private:
	generator_type m_generator;
	
};