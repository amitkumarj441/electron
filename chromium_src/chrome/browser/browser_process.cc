// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/browser_process.h"

#include "chrome/browser/printing/print_job_manager.h"
#include "content/public/browser/child_process_security_policy.h"
#include "ui/base/l10n/l10n_util.h"

#if defined(ENABLE_EXTENSIONS)
#include "atom/browser/extensions/atom_extensions_browser_client.h"
#include "atom/browser/extensions/extension_renderer_state.h"
#include "atom/common/extensions/atom_extensions_client.h"
#include "extensions/common/constants.h"
#include "extensions/common/features/feature_provider.h"
#include "ui/base/resource/resource_bundle.h"
#endif

BrowserProcess* g_browser_process = NULL;

BrowserProcess::BrowserProcess() {
  g_browser_process = this;

  print_job_manager_.reset(new printing::PrintJobManager);

#if defined(ENABLE_EXTENSIONS)
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      extensions::kExtensionScheme);
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      extensions::kExtensionResourceScheme);

  ExtensionRendererState::GetInstance()->Init();
  extensions::ExtensionsClient::Set(new extensions::AtomExtensionsClient());
  extensions_browser_client_.reset(
      new extensions::AtomExtensionsBrowserClient());
  extensions::ExtensionsBrowserClient::Set(extensions_browser_client_.get());
  // make sure everything is loaded
  extensions::FeatureProvider::GetAPIFeatures();
  extensions::FeatureProvider::GetPermissionFeatures();
  extensions::FeatureProvider::GetManifestFeatures();
  extensions::FeatureProvider::GetBehaviorFeatures();
#endif
}

BrowserProcess::~BrowserProcess() {
  #if defined(ENABLE_EXTENSIONS)
    ExtensionRendererState::GetInstance()->Shutdown();
  #endif
  g_browser_process = NULL;
}

std::string BrowserProcess::GetApplicationLocale() {
  return l10n_util::GetApplicationLocale("");
}

printing::PrintJobManager* BrowserProcess::print_job_manager() {
  return print_job_manager_.get();
}

bool BrowserProcess::IsShuttingDown() {
  return false;
}
