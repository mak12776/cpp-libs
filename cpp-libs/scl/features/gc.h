#pragma once

namespace scl
{
	namespace gc
	{
		enum class job_type : uint8_t
		{
			FCLOSE, FREE, DELETE_PNTR,
		};
		
		struct job_t
		{
			job_type type;
			union
			{
				FILE *file;
				void *pntr;
			};
		};

		struct gc_t
		{
			size_t array_index = 0;

			size_t array_size;
			job_t *job_array;

			inline void add_fclose(FILE *file)
			{
				if (array_index != array_size)
				{
					job_array[array_index].type = job_type::FCLOSE;
					job_array[array_index].file = file;
					array_index += 1;
				}
			}

			inline void add_free(void *pntr)
			{
				if (array_index != array_size)
				{
					job_array[array_index].type = job_type::FREE;
					job_array[array_index].pntr = pntr;
					array_index += 1;
				}
			}

			inline void add_delete(void *pntr)
			{
				if (array_index != array_size)
				{
					job_array[array_index].type = job_type::DELETE_PNTR;
					job_array[array_index].pntr = pntr;
					array_index += 1;
				}
			}

			inline void finish(size_t start = 0)
			{
				for (size_t index = 0; index < array_index; index += 1)
				{
					switch (job_array[index].type)
					{
					case job_type::FCLOSE:
						fclose(job_array[index].file);
						break;

					case job_type::FREE:
						free(job_array[index].pntr);
						break;

					case job_type::DELETE_PNTR:
						delete job_array[index].pntr;
						break;
					}
				}
			}
		};

		// default gc_t

		constexpr size_t default_array_size = (1 << 13); // 8192
		job_t default_array[default_array_size];
		
		constexpr gc_t default_gc{0, default_array_size, default_array};
		gc_t global_gc = default_gc;

		// functions

		constexpr size_t get_index() { return global_gc.array_index; }

		void add_fclose(FILE *file) { global_gc.add_fclose(file); }
		void add_free(void *pntr) { global_gc.add_free(pntr); }
		void add_delete(void *pntr) { global_gc.add_delete(pntr); }
		void finish(size_t start = 0) { global_gc.finish(start); }
	}
}
