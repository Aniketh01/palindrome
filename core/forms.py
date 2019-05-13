from django import forms
from core.models import IP


class addIPForm(forms.ModelForm):
    ipaddr = forms.CharField()

    class Meta:
        model = IP
        fields = ('ipaddr', )