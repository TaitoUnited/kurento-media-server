/*
 * (C) Copyright 2013 Kurento (http://kurento.org/)
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */

#include "MediaServerServiceHandler.hpp"
#include "KmsMediaServer_constants.h"
#include <gst/gst.h>

#include "types/MediaPipeline.hpp"
#include "types/MediaElement.hpp"
#include "types/Mixer.hpp"

#include "KmsMediaErrorCodes_constants.h"
#include "utils/utils.hpp"

#define GST_CAT_DEFAULT kurento_media_server_service_handler
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoMediaServerServiceHandler"

namespace kurento
{

MediaServerServiceHandler::MediaServerServiceHandler ()
{
}

MediaServerServiceHandler::~MediaServerServiceHandler ()
{
}

int32_t
MediaServerServiceHandler::getVersion ()
{
  return g_KmsMediaServer_constants.VERSION;
}

/* MediaObject */

void
MediaServerServiceHandler::keepAlive (const KmsMediaObjectRef &mediaObjectRef) throw (KmsMediaServerException)
{
  GST_TRACE ("keepAlive %" G_GUINT64_FORMAT, mediaObjectRef.id);

  try {
    mediaSet.keepAlive (mediaObjectRef);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("keepAlive %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("keepAlive %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in keepAlive");
  }

  GST_TRACE ("keepAlive %" G_GUINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::release (const KmsMediaObjectRef &mediaObjectRef) throw (KmsMediaServerException)
{
  GST_TRACE ("release %" G_GUINT64_FORMAT, mediaObjectRef.id);

  try {
    mediaSet.remove (mediaObjectRef);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("release %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("release %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in release");
  }

  GST_TRACE ("release %" G_GUINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::subscribeEvent (std::string &_return, const KmsMediaObjectRef &mediaObjectRef,
    const std::string &eventType, const std::string &handlerAddress,
    const int32_t handlerPort) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GUINT64_FORMAT, eventType.c_str (), mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    _return = mo->subscribe (eventType, handlerAddress, handlerPort);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", eventType.c_str (), mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GUINT64_FORMAT " throws KmsMediaServerException", eventType.c_str (), mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in subscribe");
  }

  GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GUINT64_FORMAT " done", eventType.c_str (), mediaObjectRef.id);
}

void
MediaServerServiceHandler::unsubscribeEvent (const KmsMediaObjectRef &mediaObjectRef, const std::string &callbackToken)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %" G_GUINT64_FORMAT, callbackToken.c_str (), mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    mo->unsubscribe (callbackToken);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", callbackToken.c_str (), mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %" G_GUINT64_FORMAT " throws KmsMediaServerException", callbackToken.c_str (), mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in subscribe");
  }

  GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %" G_GUINT64_FORMAT " done", callbackToken.c_str (), mediaObjectRef.id);
}

void
MediaServerServiceHandler::subscribeError (std::string &_return, const KmsMediaObjectRef &mediaObjectRef,
    const std::string &handlerAddress, const int32_t handlerPort)
throw (KmsMediaServerException)
{
  GST_WARNING ("TODO: implement");
}

void
MediaServerServiceHandler::unsubscribeError (const KmsMediaObjectRef &mediaObjectRef, const std::string &callbackToken)
throw (KmsMediaServerException)
{
  GST_WARNING ("TODO: implement");
}

void
MediaServerServiceHandler::sendCommand (KmsMediaCommandResult &_return, const KmsMediaObjectRef &mediaObjectRef, const KmsMediaCommand &command)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("sendCommand mediaObjectRef: %" G_GUINT64_FORMAT, mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    _return = * (mo->sendCommand (command) );
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("sendCommand mediaObjectRef: %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("sendCommand mediaObjectRef: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in sendCommand");
  }

  GST_TRACE ("sendCommand mediaObjectRef: %" G_GUINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::getParent (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaObjectRef)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;
  std::shared_ptr<KmsMediaObjectRef> parent;

  GST_TRACE ("getParent %" G_GUINT64_FORMAT, mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    parent = mo->getParent ();
    _return = *parent;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getParent %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("release %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getParent");
  }

  GST_TRACE ("getParent %" G_GUINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::getMediaPipeline (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaObjectRef)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("getMediaPipeline %" G_GUINT64_FORMAT, mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);

    if (std::dynamic_pointer_cast<MediaPipeline> (mo) ) {
      _return = *mo;
    } else {
      _return = * (mo->getParent () );
    }
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaPipeline %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaObjectRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("release %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaObjectRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getMediaPipeline");
  }

  GST_TRACE ("getMediaPipeline %" G_GUINT64_FORMAT " done", mediaObjectRef.id);
}

/* MediaPipeline */

void
MediaServerServiceHandler::createMediaPipeline (KmsMediaObjectRef &_return) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mediaPipeline;

  GST_TRACE ("createMediaPipeline");

  try {
    mediaPipeline = std::shared_ptr<MediaPipeline> (new MediaPipeline () );
    GST_DEBUG ("createMediaPipeline id: %" G_GINT64_FORMAT ", token: %s", mediaPipeline->id, mediaPipeline->token.c_str() );
    mediaSet.put (mediaPipeline);

    _return = *mediaPipeline;
  } catch (...) {
    GST_TRACE ("createMediaPipeline throws KmsMediaServerException");
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMediaPipeline");
  }

  GST_TRACE ("createMediaPipeline done");
}

void
MediaServerServiceHandler::createMediaPipelineWithParams (KmsMediaObjectRef &_return, const KmsMediaParams &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaHandler> mh;
  std::shared_ptr<MediaPipeline> mediaPipeline;

  GST_TRACE ("createMediaPipelineWithParams");

  try {
    mediaPipeline = std::shared_ptr<MediaPipeline> (new MediaPipeline (params) );
    GST_DEBUG ("createMediaPipelineWithParams id: %" G_GINT64_FORMAT ", token: %s", mediaPipeline->id, mediaPipeline->token.c_str() );
    mediaSet.put (mediaPipeline);

    _return = *mediaPipeline;
  } catch (...) {
    GST_TRACE ("createMediaPipelineWithParams throws KmsMediaServerException");
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMediaPipelineWithParams");
  }

  GST_TRACE ("createMediaPipelineWithParams done");
}

void
MediaServerServiceHandler::createMediaElement (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaPipeline,
    const std::string &elementType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<MediaElement> me;

  GST_TRACE ("createMediaElement pipeline: %" G_GUINT64_FORMAT ", type: %s", mediaPipeline.id, elementType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    me = mp->createMediaElement (elementType);
    mediaSet.put (me);

    _return = *me;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaElement pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id, elementType.c_str (), e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("createMediaElement pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException", mediaPipeline.id, elementType.c_str () );
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMediaElement");
  }

  GST_TRACE ("createMediaElement pipeline: %" G_GUINT64_FORMAT ", type: %s done", mediaPipeline.id, elementType.c_str () );
}

void
MediaServerServiceHandler::createMediaElementWithParams (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaPipeline,
    const std::string &elementType, const KmsMediaParams &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<MediaElement> me;

  GST_TRACE ("createMediaElementWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s", mediaPipeline.id, elementType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    me = mp->createMediaElement (elementType, params);
    mediaSet.put (me);

    _return = *me;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaElementWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id, elementType.c_str (), e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("createMediaElementWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException", mediaPipeline.id, elementType.c_str () );
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMediaElementWithParams");
  }

  GST_TRACE ("createMediaElementWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s done", mediaPipeline.id, elementType.c_str () );
}

void
MediaServerServiceHandler::createMediaMixer (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaPipeline,
    const std::string &mixerType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<Mixer> mixer;

  GST_TRACE ("createMediaMixer pipeline: %" G_GUINT64_FORMAT ", type: %s", mediaPipeline.id, mixerType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    mixer = mp->createMediaMixer (mixerType);
    mediaSet.put (mixer);

    _return = *mixer;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaMixer pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id, mixerType.c_str (), e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("createMediaMixer pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException", mediaPipeline.id, mixerType.c_str () );
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMediaMixer");
  }

  GST_TRACE ("createMediaMixer pipeline: %" G_GUINT64_FORMAT ", type: %s done", mediaPipeline.id, mixerType.c_str () );
}

void
MediaServerServiceHandler::createMediaMixerWithParams (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaPipeline,
    const std::string &mixerType, const KmsMediaParams &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<Mixer> mixer;

  GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s", mediaPipeline.id, mixerType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    mixer = mp->createMediaMixer (mixerType, params);
    mediaSet.put (mixer);

    _return = *mixer;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id, mixerType.c_str (), e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s throws KmsMediaServerException", mediaPipeline.id, mixerType.c_str () );
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMediaMixerWithParams");
  }

  GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GUINT64_FORMAT ", type: %s done", mediaPipeline.id, mixerType.c_str () );
}

/* MediaElement */

void
MediaServerServiceHandler::getMediaSrcs (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaElement)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSrc> > *mediaSrcs = NULL;
  std::vector< std::shared_ptr<MediaSrc> >::iterator it;

  try {
    GST_TRACE ("getMediaSrcs element: %" G_GUINT64_FORMAT, mediaElement.id);
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    mediaSrcs = me->getMediaSrcs();

    for ( it = mediaSrcs->begin() ; it != mediaSrcs->end(); ++it) {
      mediaSet.put (*it);
      _return.push_back (**it);
    }

    delete mediaSrcs;
    GST_TRACE ("getMediaSrcs element: %" G_GUINT64_FORMAT " done", mediaElement.id);
  } catch (const KmsMediaServerException &e) {
    delete mediaSrcs;
    GST_TRACE ("getMediaSrcs element: %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaElement.id, e.what () );
    throw e;
  } catch (...) {
    delete mediaSrcs;
    GST_TRACE ("getMediaSrcs element: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaElement.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getMediaSrcs");
  }
}

void
MediaServerServiceHandler::getMediaSinks (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaElement)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSink> > *mediaSinks = NULL;
  std::vector< std::shared_ptr<MediaSink> >::iterator it;

  try {
    GST_TRACE ("getMediaSinks element: %" G_GUINT64_FORMAT, mediaElement.id);
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    mediaSinks = me->getMediaSinks();

    for ( it = mediaSinks->begin() ; it != mediaSinks->end(); ++it) {
      mediaSet.put (*it);
      _return.push_back (**it);
    }

    delete mediaSinks;
    GST_TRACE ("getMediaSinks element: %" G_GUINT64_FORMAT " done", mediaElement.id);
  } catch (const KmsMediaServerException &e) {
    delete mediaSinks;
    GST_TRACE ("getMediaSinks element: %" G_GUINT64_FORMAT " throws KmsMediaServerException (%s)", mediaElement.id, e.what () );
    throw e;
  } catch (...) {
    delete mediaSinks;
    GST_TRACE ("getMediaSinks element: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaElement.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getMediaSinks");
  }
}

void
MediaServerServiceHandler::getMediaSrcsByMediaType (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaElement,
    const KmsMediaType::type mediaType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSrc> > *mediaSrcs = NULL;
  std::vector< std::shared_ptr<MediaSrc> >::iterator it;

  try {
    GST_TRACE ("getMediaSrcByType element: %" G_GUINT64_FORMAT " mediaType: %s", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    mediaSrcs = me->getMediaSrcsByMediaType (mediaType);

    for ( it = mediaSrcs->begin() ; it != mediaSrcs->end(); ++it) {
      mediaSet.put (*it);
      _return.push_back (**it);
    }

    delete mediaSrcs;
    GST_TRACE ("getMediaSrcsByType element: %" G_GUINT64_FORMAT " mediaType: %s done", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
  } catch (const KmsMediaServerException &e) {
    delete mediaSrcs;
    GST_TRACE ("getMediaSrcsByType element: %" G_GUINT64_FORMAT " mediaType: %s throws KmsMediaServerException (%s)", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType), e.what () );
    throw e;
  } catch (...) {
    delete mediaSrcs;
    GST_TRACE ("getMediaSrcsByType element: %" G_GUINT64_FORMAT " mediaType: %s throws KmsMediaServerException", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getMediaSrcsByType");
  }
}

void
MediaServerServiceHandler::getMediaSinksByMediaType (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaElement,
    const KmsMediaType::type mediaType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSink> > *mediaSinks = NULL;
  std::vector< std::shared_ptr<MediaSink> >::iterator it;

  try {
    GST_TRACE ("getMediaSinksByType element: %" G_GUINT64_FORMAT " mediaType: %s", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    mediaSinks = me->getMediaSinksByMediaType (mediaType);

    for ( it = mediaSinks->begin() ; it != mediaSinks->end(); ++it) {
      mediaSet.put (*it);
      _return.push_back (**it);
    }

    delete mediaSinks;
    GST_TRACE ("getMediaSinksByType element: %" G_GUINT64_FORMAT " mediaType: %s done", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
  } catch (const KmsMediaServerException &e) {
    delete mediaSinks;
    GST_TRACE ("getMediaSinksByType element: %" G_GUINT64_FORMAT " mediaType: %s throws KmsMediaServerException (%s)", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType), e.what () );
    throw e;
  } catch (...) {
    delete mediaSinks;
    GST_TRACE ("getMediaSinksByType element: %" G_GUINT64_FORMAT " mediaType: %s throws KmsMediaServerException", mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getMediaSinksByType");
  }
}

void
MediaServerServiceHandler::getMediaSrcsByFullDescription (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaElement,
    const KmsMediaType::type mediaType, const std::string &description)
throw (KmsMediaServerException)
{
  GST_WARNING ("TODO: implement");
}

void
MediaServerServiceHandler::getMediaSinksByFullDescription (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaElement,
    const KmsMediaType::type mediaType, const std::string &description)
throw (KmsMediaServerException)
{
  GST_WARNING ("TODO: implement");
}

/* MediaPad */

void
MediaServerServiceHandler::getMediaElement (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaPadRef) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPad> pad;

  try {
    GST_TRACE ("getMediaElement of pad: %" G_GUINT64_FORMAT, mediaPadRef.id);
    pad = mediaSet.getMediaObject<MediaPad> (mediaPadRef);
    _return = * (pad->getMediaElement () );
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaElement of pad: %" G_GUINT64_FORMAT " throws KmsMediaServerException(%s)", mediaPadRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("getMediaElement of pad: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaPadRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getMediaElement");
  }

  GST_TRACE ("getMediaElement of pad: %" G_GUINT64_FORMAT " done", mediaPadRef.id);
}

/* MediaSrc */

void
MediaServerServiceHandler::connect (const KmsMediaObjectRef &mediaSrc, const KmsMediaObjectRef &mediaSink) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSrc> src;
  std::shared_ptr<MediaSink> sink;

  try {
    GST_TRACE ("connect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT, mediaSrc.id, mediaSink.id);
    src = mediaSet.getMediaObject<MediaSrc> (mediaSrc);
    sink = mediaSet.getMediaObject<MediaSink> (mediaSink);
    src->connect (sink);
    GST_TRACE ("connect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT " done", mediaSrc.id, mediaSink.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("connect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT " throws KmsMediaServerException(%s)", mediaSrc.id, mediaSink.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("connect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaSrc.id, mediaSink.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in connect");
  }
}

void
MediaServerServiceHandler::disconnect (const KmsMediaObjectRef &mediaSrc, const KmsMediaObjectRef &mediaSink) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSrc> src;
  std::shared_ptr<MediaSink> sink;

  try {
    GST_TRACE ("disconnect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT, mediaSrc.id, mediaSink.id);
    src = mediaSet.getMediaObject<MediaSrc> (mediaSrc);
    sink = mediaSet.getMediaObject<MediaSink> (mediaSink);
    src->disconnect (sink);
    GST_TRACE ("disconnect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT " done", mediaSrc.id, mediaSink.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("disconnect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT " throws KmsMediaServerException(%s)", mediaSrc.id, mediaSink.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("disconnect src: %" G_GUINT64_FORMAT " sink: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaSrc.id, mediaSink.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in disconnect");
  }
}

void
MediaServerServiceHandler::getConnectedSinks (std::vector<KmsMediaObjectRef> & _return, const KmsMediaObjectRef &mediaSrc) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSrc> src;
  std::vector < std::shared_ptr<MediaSink> > *mediaSinks = NULL;
  std::vector< std::shared_ptr<MediaSink> >::iterator it;

  try {
    GST_TRACE ("getConnectedSinks src: %" G_GUINT64_FORMAT, mediaSrc.id);
    src = mediaSet.getMediaObject<MediaSrc> (mediaSrc);
    mediaSinks = src->getConnectedSinks();

    for ( it = mediaSinks->begin() ; it != mediaSinks->end(); ++it) {
      mediaSet.put (*it);
      _return.push_back (**it);
    }

    delete mediaSinks;
    GST_TRACE ("getConnectedSinks src: %" G_GUINT64_FORMAT " done", mediaSrc.id);
  } catch (const KmsMediaServerException &e) {
    delete mediaSinks;
    GST_TRACE ("getConnectedSinks src: %" G_GUINT64_FORMAT " throws KmsMediaServerException(%s)", mediaSrc.id, e.what () );
    throw e;
  } catch (...) {
    delete mediaSinks;
    GST_TRACE ("getConnectedSinks src: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaSrc.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getConnectedSinks");
  }
}

/* MediaSink */

void
MediaServerServiceHandler::getConnectedSrc (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaSinkRef) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSink> sink;

  try {
    GST_TRACE ("getConnectedSrc sink: %" G_GUINT64_FORMAT, mediaSinkRef.id);
    sink = mediaSet.getMediaObject<MediaSink> (mediaSinkRef);
    _return = * (sink->getConnectedSrc() );
    GST_TRACE ("getConnectedSrc sink: %" G_GUINT64_FORMAT " done", mediaSinkRef.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getConnectedSrc sink: %" G_GUINT64_FORMAT " throws KmsMediaServerException(%s)", mediaSinkRef.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("getConnectedSrc sink: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mediaSinkRef.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in getConnectedSrc");
  }
}

/* Mixer */

void
MediaServerServiceHandler::createMixerEndPoint (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mixer) throw (KmsMediaServerException)
{
  std::shared_ptr<Mixer> m;
  std::shared_ptr<MixerEndPoint> mixerEndPoint;

  GST_TRACE ("createMixerEndPoint mixer: %" G_GUINT64_FORMAT, mixer.id);

  try {
    m = mediaSet.getMediaObject<Mixer> (mixer);
    mixerEndPoint = m->createMixerEndPoint();
    mediaSet.put (mixerEndPoint);

    _return = *mixerEndPoint;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMixerEndPoint mixer: %" G_GUINT64_FORMAT " throws KmsMediaServerException(%s)", mixer.id, e.what () );
    throw e;
  } catch (...) {
    GST_TRACE ("createMixerEndPoint mixer: %" G_GUINT64_FORMAT " throws KmsMediaServerException", mixer.id);
    throw createKmsMediaServerException (g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in createMixerEndPoint");
  }

  GST_TRACE ("createMixerEndPoint mixer: %" G_GUINT64_FORMAT " done", mixer.id);
}

void
MediaServerServiceHandler::createMixerEndPointWithParams (KmsMediaObjectRef &_return, const KmsMediaObjectRef &mixer, const KmsMediaParams &params)
throw (KmsMediaServerException)
{
  GST_WARNING ("TODO: implement");
}

MediaServerServiceHandler::StaticConstructor MediaServerServiceHandler::staticConstructor;

MediaServerServiceHandler::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
      GST_DEFAULT_NAME);
}

} // kurento
