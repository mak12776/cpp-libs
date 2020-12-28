#pragma once

namespace scl
{
	// size manager

	// --- header definitions ---

	typedef void(*size_manager_t)(size_t &size);

	template <size_t initial_size>
	void double_size_manager(size_t &size);

	template <size_t initial_size>
	void half_size_manager(size_t &size);

	template <size_t initial_size, size_t adder_size>
	void adder_size_manager(size_t &size);

	constexpr size_manager_t default_size_manager = half_size_manager<16>;

	template <typename data_type, size_t array_size>
	struct static_array_t;

	template <typename data_type>
	struct dynamic_array_t;

	template <typename data_type, size_t array_size>
	struct static_list_t;

	template <typename data_type, size_manager_t size_manager = default_size_manager>
	struct dynamic_list_t;



	// --- source declerations ---

	template <size_t initial_size>
	void double_size_manager(size_t &size)
	{
		if (size < initial_size)
		{
			size = initial_size;
			return;
		}
		math::safe_mul(size, (size_t)2, size); ERR_CHECK_NO_RETURN;
	}

	template <size_t initial_size>
	void half_size_manager(size_t &size)
	{
		if (size < initial_size)
		{
			size = initial_size;
			return;
		}
		math::safe_add(size, size / 2, size); ERR_CHECK_NO_RETURN;
	}

	template <size_t initial_size, size_t adder_size>
	void adder_size_manager(size_t &size)
	{
		if (size < initial_size)
		{
			size = initial_size;
			return;
		}
		math::safe_add(size, adder_size, size); ERR_CHECK_NO_RETURN;
	}

	// const array

	template <typename value_type = char>
	struct const_array
	{
		const value_type *const pntr;
		const size_t len;

		template <size_t size>
		const_array(const value_type(&pntr)[size]) : pntr(pntr), len(size - 1)
		{ }
	};

	// dynamic array

	template <typename data_type, size_t array_size>
	struct static_array_t
	{
		data_type pntr[array_size];

		constexpr inline size_t size() { return array_size; }
	};


	template <typename data_type>
	struct dynamic_array_t
	{
		typedef data_type value_type;

		typedef data_type &reference;
		typedef const data_type &const_reference;

		typedef data_type *pointer;

		pointer pntr;
		size_t array_size;

		dynamic_array_t() : pntr(nullptr), array_size(0) {}
		dynamic_array_t(data_type *pntr, size_t size) : pntr(pntr), array_size(size) {}

		// members functions

		constexpr size_t size() { return this->array_size; }
		constexpr bool empty() { return this->array_size == 0; }
		constexpr pointer data() { return this->pntr; }

		// operators

		inline reference operator[](size_t index)
		{
			return pntr[index];
		}

		// cast operators

		template <size_manager_t size_manager>
		inline void as_dynamic_list(dynamic_list_t<data_type, size_manager> &list)
		{
			list.pntr = this->pntr;
			list.size = this->array_size;
			list.len = this->array_size;
		}

		inline operator dynamic_list_t<data_type> const ()
		{
			dynamic_list_t<data_type> result;
			result.pntr = this->pntr;
			result.len = this->array_size;
			result.size = this->array_size;
			return result;
		}

		// iterators

		typedef data_type *iterator;
		typedef const data_type *const_iterator;

		constexpr iterator begin() { return this->pntr; } 
		constexpr const_iterator cbegin() { return this->pntr; }
		constexpr iterator end() { return this->pntr + this->array_size; }
		constexpr const_iterator cend() { return this->pntr + this->array_size; }


		// allocation

		inline void allocate(size_t size)
		{
			pntr = mem::safe_malloc_array<data_type>(size); ERR_CHECK_RETURN;
			this->array_size = size;
		}

		inline void allocate_fread_all(FILE *file)
		{
			io::malloc_fread_array_all<data_type>(file, &pntr, array_size); ERR_CHECK_NO_RETURN;
		}

		inline void allocate_fopen_fread_all(const char *name)
		{
			io::malloc_fopen_fread_array_all<data_type>(name, &pntr, array_size); ERR_CHECK_NO_RETURN;
		}

		inline void reallocate(size_t size)
		{
			mem::safe_realloc_array<data_type>(&pntr, size); ERR_CHECK_RETURN;
			this->array_size = size;
		}

		inline void deallocate()
		{
			mem::free(pntr);
			this->pntr = nullptr;
			this->array_size = 0;
		}


		// fread, fwrite

		inline size_t fread(FILE *file)
		{
			size_t read_number = io::fread_array<data_type>(pntr, array_size, file); ERR_CHECK_NO_RETURN;
			return read_number;
		}

		inline size_t fwrite(FILE *file)
		{
			size_t write_number = io::fwrite_array<data_type>(pntr, array_size, file); ERR_CHECK_NO_RETURN;
			return write_number;
		}

		// fopen fread, fwrite

		inline size_t fopen_fread(const char *name)
		{
			size_t read_number = io::fopen_fread(name, pntr, array_size); ERR_CHECK_NO_RETURN;
			return read_number;
		}

		inline size_t fopen_fwrite(const char *name)
		{
			size_t write_number = io::fopen_fwrite(name, pntr, array_size); ERR_CHECK_NO_RETURN;
			return write_number;
		}
	};

	// list

	template <typename data_type, size_t array_size>
	struct static_list_t
	{
		data_type pntr[array_size];
		size_t len;
	};

	template <typename data_type, size_manager_t size_manager>
	struct dynamic_list_t
	{
		data_type *pntr;
		size_t len;
		size_t size;

	private:
		inline void reset()
		{
			this->pntr = nullptr;
			this->len = 0;
			this->size = 0;
		}

	public:
		dynamic_list_t() { reset(); }

		// operators

		inline operator dynamic_array_t<data_type> const ()
		{
			dynamic_array_t<data_type> result;
			result.pntr = this->pntr;
			result.len = this->size;
			result.size = this->size;
			return result;
		}

		// allocation

		inline void allocate(size_t size)
		{
			this->pntr = mem::safe_malloc_array<data_type>(size); ERR_CHECK_RETURN;
			this->len = 0;
			this->size = size;
		}

		inline void reallocate(size_t size)
		{
			mem::safe_realloc_array<data_type>(&pntr, size); ERR_CHECK_RETURN;

			if (this->len > size)
				this->len = size;
			this->size = size;
		}

		inline void deallocate()
		{
			mem::free(pntr); reset();
		}
	};

	// linked array

	template <typename data_type, size_t array_size>
	struct linked_array_t
	{
		typedef data_type& reference;
		typedef const data_type& const_reference;

		struct node_t
		{
			data_type pntr[array_size];
			node_t *next;

			constexpr inline size_t size() { return array_size; }
			constexpr data_type& operator[](size_t index) { return pntr[index]; }
		};

		size_t len;
		node_t *first;
		node_t *last;

		linked_array_t() : len(0), first(nullptr), last(nullptr) { }

		inline bool append(reference value)
		{
			if (len == 0)
			{
				first = (node_t *)mem::malloc(sizeof node_t);
				if (first == nullptr)
					return true;

				last = first;
			}
			else if (len % array_size == 0)
			{
				last->next = (node_t *)mem::malloc(sizeof node_t);
				if (last->next == nullptr)
					return true;

				last = last->next;
			}

			last->pntr[len % array_size] = value;
			len += 1;
			return false;
		}

		inline void safe_append(const_reference value)
		{
			if (len == 0)
			{
				last = first = mem::safe_malloc_type<node_t>(); 
				ERR_CHECK_RETURN;
			}
			else if (len % array_size == 0)
			{
				last->next = mem::safe_malloc_type<node_t>(); 
				ERR_CHECK_RETURN;

				last = last->next;
			}

			last->pntr[len % array_size] = value;
			len += 1;
		}
	};

	template <typename data_type>
	struct linked_list_t
	{
		typedef data_type& reference;
		typedef const data_type& const_reference;
	};


	// sub types

	using c_ustring_t = const_array<unsigned char>;
	using c_string_t = const_array<char>;

	typedef dynamic_array_t<byte> buffer_t;
	typedef dynamic_array_t<ubyte> ubuffer_t;
	typedef dynamic_array_t<void> vbuffer_t;
}