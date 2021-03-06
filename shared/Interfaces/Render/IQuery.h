#pragma once

ZN_INTERFACE ZN_API IQuery
{
	enum class ZN_API QueryType
	{
		Timer,
		CountSamples,
		CountSamplesPredicate,
		CountPrimitives,
		CountTransformFeedbackPrimitives,
	};

	struct ZN_API QueryResult
	{
		union
		{
			double   ElapsedTime;                   // Valid for QueryType::Timer queries. Returns the elapsed time in seconds between IQuery::Begin and IQuery::End.
			uint64_t PrimitivesGenerated;           // Valid for QueryType::CountPrimitives. Returns the number of primitives generated between IQuery::Begin and IQuery::End.
			uint64_t TransformFeedbackPrimitives;   // Valid for QueryType::CountTransformFeedbackPrimitives. Returns the number of primtives written to stream out or transform feedback buffers.
			uint64_t NumSamples;                    // Valid for QueryType::CountSamples. Returns the number of samples written by the fragment shader between IQuery::Begin and IQuery::End.
			bool     AnySamples;                    // Valid for QueryType::CountSamplesPredicate. Returns true if any samples were written by the fragment shader between IQuery::Begin and IQuery::End.
		};

		bool IsValid; // Are the results of the query valid? You should check this before using the value.
	};

	virtual ~IQuery() {}

	virtual void Begin(int64_t frame = 0L) = 0;
	virtual void End(int64_t frame = 0L) = 0;
	virtual bool QueryResultAvailable(int64_t frame = 0L) = 0;

	/**
	 * Retrieve the query results from the query object.
	 * The type of result returned is determined by the query type.
	 * Retrieving the results could result in a GPU stall if you request
	 * the current frame's result because the CPU has to wait until all of the
	 * GPU commands that were enqued on the GPU before IQuery::End was called.
	 * You should instead request the query results from 2 or 3 frames previous
	 * (if you can wait a few frames for the result). If you absolutely need the current frame's result,
	 * you can query the current frames result but will almost always result in a GPU stall.
	 * IMPORTANT: Be sure to check the QueryResult::IsValid flag before using the result.
	 */
	virtual QueryResult GetQueryResult(int64_t frame = 0L) = 0;

	/**
	 * GPU queries can generally be multi-buffered to reduce
	 * GPU stalls when getting the query result. If the implementation provides
	 * multi-buffered queries this method will return the number of buffered query
	 * results that are available.
	 */
	virtual uint8_t GetBufferCount() const = 0;
};
