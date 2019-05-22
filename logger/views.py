# -*- coding: utf-8 -*-
from __future__ import unicode_literals

import os

from django.contrib.auth import authenticate, login
from django.urls import reverse_lazy
from django.views import generic
from django.shortcuts import render
from django.core.management import call_command
from .forms import loggerForm
from .models import loggerModel
from django.http import HttpResponse
from twilio.rest import Client


class add_logger(generic.CreateView):
    template_name = 'logger.html'

    def get(self, request):
        form = loggerForm()
        return render(request, self.template_name, {'form': form})
    
    def post(self, request):
        form= logger(request.POST)
        args = {'form': form, 'text': text}
        return render(request, self.template_name, args)
    
    def search(request):
        if 'q' in request.GET:
            account_sid = "AC5b9acec6a476697f62191cf40a5d842d"
            auth_token  = "594f63482e4560300f6a9a6c20452dfe"
            client = Client(account_sid, auth_token)
            message = client.messages.create(
                body="Mamaaaa please?! I love you <3",
                to="+917012418862",
                from_="+12564729923",
            )
            print(message.sid)
            message = 'You searched for: %r' % request.GET['q']
        else:
            message = 'You submitted an empty form.'
        return HttpResponse(message)
    
    def profile_page(request, username=None):
        user = User.objects.get(username=username)
        message = request.GET.get('message')

    def search_form(request):
        return render(request, 'logger/search-form.html')