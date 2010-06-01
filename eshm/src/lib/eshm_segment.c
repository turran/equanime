#include "Eshm.h"
#include "eshm_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eshm_Segment
{
	char *id;
	void *data;
	int shmid;
	Eina_Bool locked;
	size_t size;
};

static void _error_to_eina(Eshm_Error err)
{
	int error = 0;

	switch (err)
	{
		case ESHM_ERR_ACCESS:
			error = ESHM_ERROR_ACCESS;
			break;
		case ESHM_ERR_EXIST:
			error = ESHM_ERROR_EXIST;
			break;
		case ESHM_ERR_NEXIST:
			error = ESHM_ERROR_NEXIST;
			break;
		case ESHM_ERR_CODEC:
			error = ESHM_ERROR_CODEC;
			break;
	}
	if (error)
	{
		eina_error_set(error);
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new segment.
 * This function is a blocking function, it will block until the request is
 * done. In case of error the Eina error variable is set.
 * @param id The unique identifier for this segment
 * @param size The size in bytes of the segment
 * @return The created segment or NULL if there was an error
 */
EAPI Eshm_Segment * eshm_segment_new(const char *id, size_t size)
{
	Eshm_Segment *s;
	Eshm_Message_Segment_New m;
	Eshm_Reply_Segment_New *r;
	Eshm_Error error;

	m.id = id;
	m.size = size;

	/* send the message to the daemon */
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_NEW, &m, 0, (void **)&r);
	if (error)
	{
		_error_to_eina(error);
		WRN("Unable to create new segment with id = %s [%s]", id, eina_error_msg_get(eina_error_get()));
		return NULL;
	}
	/* allocate the new segment and give it back to user */
	s = calloc(1, sizeof(Eshm_Segment));

	s->shmid = r->shmid;
	s->id = strdup(id);
	s->data = shmat(s->shmid, NULL, 0);
	s->size = size;

	free(r);

	DBG("New segment of id %s created with numeric id %d", s->id, r->shmid);
	return s;
}
/**
 * Gets a previously created segment or creates it.
 * This function is a blocking function, it will block until the request is
 * done. In case of error the Eina error variable is set.
 * @param id The unique identifier for this segment
 * @param size The size in bytes of the segment
 * @param create If EINA_TRUE it will create the segment if it is not created already
 * @return The created segment or NULL if there was an error
 */
EAPI Eshm_Segment * eshm_segment_get(const char *id, size_t size, Eina_Bool create)
{
	Eshm_Segment *s;
	Eshm_Message_Segment_Get m;
	Eshm_Reply_Segment_Get *r;
	Eshm_Error error;

	if (!id && !create)
	{
		return NULL;
	}

	m.id = id;

	/* send the message to the daemon */
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_GET, &m, 0, (void **)&r);
	if (error)
	{
		if (!create && (error == ESHM_ERROR_NEXIST))
			return eshm_segment_new(id, size);
		else
		{
			_error_to_eina(error);
			WRN("Unable to request a segment with id = %s", id);
			return NULL;
		}
	}
	else
	{
		/* allocate the new segment and give it back to user */
		DBG("Requested segment id = %d", r->shmid);

		s = calloc(1, sizeof(Eshm_Segment));

		s->shmid = r->shmid;
		s->size = r->size;
		s->id = strdup(id);
		s->data = shmat(s->shmid, NULL, 0);

		free(r);
		return s;
	}
}
/**
 * Deletes a segment.
 * @param s The segment to delete
 */
EAPI void eshm_segment_delete(Eshm_Segment *s)
{
	Eshm_Message_Segment_Delete m;

	assert(s);
	m.id = s->id;

	eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_DELETE, &m, 0, NULL);
	DBG("Segment with id \"%s\" deleted", s->id);
}
/**
 * Locks the segment for read or write
 */
EAPI Eina_Bool eshm_segment_lock(Eshm_Segment *s, Eina_Bool write)
{
	Eshm_Message_Segment_Lock m;
	Eshm_Error error;

	assert(s);
	if (s->locked == EINA_TRUE)
		return EINA_TRUE;

	m.id = s->id;
	m.write = write;
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_LOCK, &m, 0, NULL);

	if (error)
	{
		_error_to_eina(error);
		WRN("Unable to lock segment with id \"%s\"", s->id);
		return EINA_FALSE;
	}
	else
	{
		DBG("Segment with id \"%s\" locked", s->id);
		s->locked = EINA_TRUE;
		return EINA_TRUE;
	}
}
/**
 *  Unlocks the segment for read or write
 */
EAPI void eshm_segment_unlock(Eshm_Segment *s)
{
	Eshm_Message_Segment_Unlock m;

	assert(s);
	if (s->locked != EINA_TRUE)
		return;

	m.id = s->id;
	eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_UNLOCK, &m, 0, NULL);
	DBG("Segment with id \"%s\" unlocked\n", s->id);
}
/**
 * Gets the memory pointer associated with this segment
 * @param s The segment to get the data from
 * @return The memory pointer
 */
EAPI void * eshm_segment_data_get(Eshm_Segment *s)
{
	assert(s);

	return s->data;
}

/**
 * Gets the size of a segment
 * @param s The segment to get the size from
 * @return The size of the sement
 */
EAPI size_t eshm_segment_size_get(Eshm_Segment *s)
{
	return s->size;
}

/**
 * Gets all the available segments registered on the server
 */
EAPI Eina_Bool eshm_segment_all_get(void (*callback)(Eshm_Segment *s))
{

}
