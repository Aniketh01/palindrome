# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.contrib.auth import authenticate, login
from django.urls import reverse_lazy
from django.views import generic
from django.shortcuts import render
from .forms import addIPForm


class add_IP(generic.CreateView):
    template_name = 'add_ip.html'

    def get(self, request):
            form = addIPForm()
            return render(request, self.template_name, {'form': form})

    def post(self, request):
        form = addIPForm(request.POST)
        if form.is_valid():
            ipaddr = form.save(commit=False)
            ipaddr.user = request.user
            ipaddr.save()

            text = form.cleaned_data['ipaddr']
            form = addIPForm()

        args = {'form': form, 'text': text}
        return render(request, self.template_name, args)

